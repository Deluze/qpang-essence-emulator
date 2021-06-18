#include "QpangConnection.h"

#include <iostream>
#include <boost/asio.hpp>

#include "core/communication/PacketHandler.h"
#include "core/communication/server/QpangServer.h"
#include "core/communication/PacketHandler.h"

QpangConnection::QpangConnection(tcp::socket&& socket, uint32_t connectionId) :
	m_socket(std::move(socket)),
	m_isConnected(true),
	m_isInitialized(false),
	m_timeLastPackReceived(0),
	m_connectionId(connectionId),
	m_packetHandler(nullptr)
{
	m_socket.non_blocking(true);
}

void QpangConnection::setDisconnectHandler(disconnect_handler closeCallback)
{
	m_closeCallback = closeCallback;
}

void QpangConnection::setDataHandler(data_handler dataCallback)
{
	m_dataCallback = dataCallback;
}

void QpangConnection::read()
{
	if (!m_isConnected)
		return;

	std::lock_guard<std::recursive_mutex> m(m_receiveMutex);

	try
	{
		boost::asio::async_read(
			m_socket,
			boost::asio::buffer(&m_packet.header, 4),
			boost::bind(&QpangConnection::onHeaderRead, shared_from_this(), boost::asio::placeholders::error)
		);
	}
	catch (const boost::system::error_code& e)
	{
		std::cout << "[QpangConnection::read] An exception occurred: " << e.message() << std::endl;
	}
}

void QpangConnection::send(const ServerPacket& packet)
{
	if (!m_isConnected)
		return;

	auto pack = packet;
	
	pack.writePayloadHeader();
	pack.writeChecksum();

	std::vector<char> sendBuf;

	sendBuf.resize(sizeof(Packet::Header) + pack.payload.size());

	pack.header.length = static_cast<uint16_t>(sendBuf.size());
	pack.header.isEncrypted = true;

	memcpy(sendBuf.data(), &pack.header, 4);
	memcpy(sendBuf.data() + 4, pack.payload.data(), 4);

	if (pack.payload.size() > 4)
		std::copy(pack.payload.cbegin() + 4, pack.payload.cend(), sendBuf.begin() + 8);

	std::vector<char> target{ sendBuf.cbegin() + 4, sendBuf.cend() };

	m_blowfishInstance.Initialize(pack.getCryptoType());
	m_blowfishInstance.Encrypt(target, pack.getCryptoType());

	if (sendBuf.size() != target.size())
		sendBuf.resize(4 + target.size());

	std::copy(target.cbegin(), target.cend(), sendBuf.begin() + 4);

	write(sendBuf);
}

void QpangConnection::close()
{
	if (!m_isConnected)
		return;

	m_isConnected = false;

	try
	{
		if (m_closeCallback)
			m_closeCallback(shared_from_this());
	}
	catch (const std::exception& e)
	{
		std::cout << "[QpangConnection::close] An exception occurred: " << e.what() << "\n";
	}

	try
	{
		if (m_socket.is_open())
		{
			m_socket.shutdown(tcp::socket::shutdown_both);
			m_socket.close();
		}
	}
	catch (const boost::system::error_code& e)
	{
		std::cout << "[QpangConnection::close] An exception occurred: " << e.message() << "\n";
	}
}

uint32_t QpangConnection::getId()
{
	return m_connectionId;
}

void QpangConnection::setIsInitialized()
{
	m_isInitialized = true;
}

bool QpangConnection::isInitialized()
{
	return m_isInitialized;
}

time_t QpangConnection::getLastPacketReceived()
{
	return m_timeLastPackReceived;
}

std::array<char, 4> QpangConnection::getKeyPart()
{
	m_blowfishInstance.Initialize(BlowfishInstance::CryptoType::AUTHENTICATED);

	std::array<char, 4> keyPart;
	std::memcpy(&keyPart, m_blowfishInstance.GetKeyPart().data(), sizeof(keyPart));

	return keyPart;
}

void QpangConnection::setPacketHandler(PacketHandler* packetHandler)
{
	m_packetHandler = packetHandler;
}

void QpangConnection::setPlayer(std::shared_ptr<Player> player)
{
	m_player = player;
}

std::shared_ptr<Player> QpangConnection::getPlayer()
{
	return m_player;
}

void QpangConnection::onHeaderRead(const boost::system::error_code& e)
{
	try
	{
		if (!m_isConnected)
			return;

		if (e)
			return close();

		std::lock_guard<std::recursive_mutex> m(m_receiveMutex);

		const size_t packetLength = m_packet.header.length - sizeof(Packet::Header);

		if (packetLength >= 8096)
			return;
		
		// resize the buffer so the payload fits in
		m_packet.payload.resize(packetLength);

		try
		{
			boost::asio::async_read(
				m_socket,
				boost::asio::buffer(m_packet.payload, m_packet.payload.size()),
				boost::bind(&QpangConnection::onPayloadRead, shared_from_this(), boost::asio::placeholders::error)
			);
		}
		catch (const boost::system::error_code& e)
		{
			std::cout << "[QpangConnection::onPacketReceive] An exception occurred: " << e.message() << std::endl;
		}
	}
	catch (const std::exception& ee)
	{
		std::cout << "[QPangConnection::onPacketReceive]: An exception occurred: " << ee.what() << std::endl;
	}
}

void QpangConnection::onPayloadRead(const boost::system::error_code& e)
{
	if (!m_isConnected)
		return;

	if (e)
	{
		std::cout << e.message() << std::endl;
		return close();
	}

	m_timeLastPackReceived = time(NULL);

	std::lock_guard<std::recursive_mutex> m(m_receiveMutex);

	if (m_packet.isEncrypted())
	{
		BlowfishInstance::CryptoType type = m_blowfishInstance.publicBfInitialized
			? BlowfishInstance::CryptoType::AUTHENTICATED
			: BlowfishInstance::CryptoType::PUBLIC;

		m_blowfishInstance.Initialize(type);
		m_blowfishInstance.Decrypt(m_packet.payload, type);
	}

	m_packetHandler->handle(shared_from_this(), m_packet);

	read();
}

void QpangConnection::write(std::vector<char> buff)
{
	try
	{
		boost::asio::async_write(
			m_socket,
			boost::asio::buffer(buff.data(), buff.size()),
			boost::bind(&QpangConnection::onWriteFinish, shared_from_this(), boost::asio::placeholders::error)
		);
	}
	catch (const boost::system::error_code& ec)
	{
		std::cout << "[QpangConnection::write]: An exception occurred: " << ec.message() << std::endl;
		close();
	}
}

void QpangConnection::onWriteFinish(const boost::system::error_code& e)
{
	if (e)
	{
		std::cout << "[QpangConnection::onWriteFinish]: An error occurred: " << e.message() << std::endl;
		close();
	}
}

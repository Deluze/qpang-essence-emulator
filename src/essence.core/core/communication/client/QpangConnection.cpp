#include "QpangConnection.h"

#include <iostream>
#include <boost/asio.hpp>

#include "core/communication/PacketHandler.h"
#include "core/communication/server/QpangServer.h"

QpangConnection::QpangConnection(tcp::socket&& socket, const uint32_t connectionId) :
	m_socket(std::move(socket)),
	m_isConnected(true),
	m_isInitialized(false),
	m_packetHandler(nullptr),
	m_playerId(0),
	m_timeLastPackReceived(0),
	m_connectionId(connectionId)
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
		std::cout << "An exception occurred: " << e.message() << std::endl;
	}
}

void QpangConnection::send(const ServerPacket& packet)
{
	if (!m_isConnected)
		return;

	auto pack = packet;

	// Adds 4 bytes to the payload
	// first 2 (short) bytes is the payloadlength (payload.size + size of payload header)
	// next 2 (short) bytes is the packet id
	pack.writePayloadHeader();
	// Writes the checksum to the packet.
	pack.writeChecksum();

	std::vector<char> sendBuf;

	// Resize the send buffer to the size of the packet header (4 bytes, packet length (2 bytes) and encrypted (1 byte) and unknown (1byte)
	// and the size of the payload (NOTE: this payload exists out of the payload data, size and packet id.
	sendBuf.resize(sizeof(Packet::Header) + pack.payload.size());

	// Set the packet header length to the sendbuffer size.
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
	{
		return;
	}

	printf("(QpangConnection::close) Disconnecting player (id = %u).\n", m_playerId);

	m_isConnected = false;

	try
	{
		if (m_closeCallback)
			m_closeCallback(shared_from_this());
	}
	catch (const std::exception& e)
	{
		std::cout << "An exception occurred: " << e.what() << "\n";
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
		std::cout << "An exception occurred: " << e.message() << "\n";
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

void QpangConnection::setPlayer(std::shared_ptr<Player> player, uint32_t playerId)
{
	m_player = player;
	m_playerId = playerId;
}

std::shared_ptr<Player> QpangConnection::getPlayer()
{
	return m_player;
}

void QpangConnection::onHeaderRead(const boost::system::error_code& ec)
{
	try
	{
		if (!m_isConnected)
			return;

		const auto errorMessage = ec.message();

		if (ec)
		{
			if (ec == boost::asio::error::eof)
			{
				/*printf("(QpangConnection::onHeaderRead) An error occurred for player (id = %u).\n", m_playerId);
				printf("(QpangConnection::onHeaderRead) An end of line error occurred, we gracefully ignore it.\n");
				printf("(QpangConnection::onHeaderRead) Corresponding end_of_file error message: %s.\n", errorMessage.c_str());*/
			}
			else if (ec == boost::asio::error::timed_out)
			{
				printf("(QpangConnection::onHeaderRead) An error occurred for player (id = %u).\n", m_playerId);
				printf("(QpangConnection::onHeaderRead) A time out error occurred, we gracefully ignore it.\n");
				printf("(QpangConnection::onHeaderRead) Corresponding time_out error message: %s.\n", errorMessage.c_str());
			}
			else if (errorMessage == "The semaphore timeout period has expired")
			{
				printf("(QpangConnection::onHeaderRead) An error occurred for player (id = %u).\n", m_playerId);
				printf("(QpangConnection::onHeaderRead) Received a semaphore timeout period, we gracefully ignore it.\n");
			}
			else
			{
				printf("(QpangConnection::onHeaderRead) An error occurred for player (id = %u).\n", m_playerId);
				printf("(QpangConnection::onHeaderRead) Received an unknown error, we should close the connection.\n");
				printf("(QpangConnection::onHeaderRead) Corresponding unknown error message: %s.\n", errorMessage.c_str());

				close();

				return;
			}
		}

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
			printf("(QpangConnection::onHeaderRead) (Async read attempt) An exception occurred: %s.\n", ec.message().c_str());
		}
	}
	catch (const std::exception& ee)
	{
		printf("(QpangConnection::onHeaderRead) (Any exception) An exception occurred: %s.\n", ee.what());
	}
}

void QpangConnection::onPayloadRead(const boost::system::error_code& ec)
{
	if (!m_isConnected)
		return;

	const auto errorMessage = ec.message();

	if (ec)
	{
		if (ec == boost::asio::error::eof)
		{
			/*printf("(QpangConnection::onPayloadRead) An error occurred for player (id = %u).\n", m_playerId);
			printf("(QpangConnection::onPayloadRead) An end of line error occurred, we gracefully ignore it.\n");
			printf("(QpangConnection::onPayloadRead) Corresponding end_of_file error message: %s.\n", errorMessage.c_str());*/
		}
		else if (ec == boost::asio::error::timed_out)
		{
			printf("(QpangConnection::onPayloadRead) An error occurred for player (id = %u).\n", m_playerId);
			printf("(QpangConnection::onPayloadRead) A time out error occurred, we gracefully ignore it.\n");
			printf("(QpangConnection::onPayloadRead) Corresponding time_out error message: %s.\n", errorMessage.c_str());
		}
		else if (errorMessage == "The semaphore timeout period has expired")
		{
			printf("(QpangConnection::onPayloadRead) An error occurred for player (id = %u).\n", m_playerId);
			printf("(QpangConnection::onPayloadRead) Received a semaphore timeout period, we gracefully ignore it.\n");
		}
		else
		{
			printf("(QpangConnection::onPayloadRead) An error occurred for player (id = %u).\n", m_playerId);
			printf("(QpangConnection::onPayloadRead) Received an unknown error, we should close the connection.\n");
			printf("(QpangConnection::onPayloadRead) Corresponding unknown error message: %s.\n", errorMessage.c_str());

			close();

			return;
		}
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
		printf("(QpangConnection::write) An exception occurred: %s.\n", ec.message().c_str());

		close();
	}
}

void QpangConnection::onWriteFinish(const boost::system::error_code& ec)
{
	const auto errorMessage = ec.message();

	if (ec)
	{
		if (ec == boost::asio::error::eof)
		{
			//printf("(QpangConnection::onWriteFinish) An error occurred for player (id = %u).\n", m_playerId);
			//printf("(QpangConnection::onWriteFinish) An end of line error occurred, we gracefully ignore it.\n");
			//printf("(QpangConnection::onWriteFinish) Corresponding end_of_file error message: %s.\n", errorMessage.c_str());
		}
		else if (ec == boost::asio::error::timed_out)
		{
			printf("(QpangConnection::onWriteFinish) An error occurred for player (id = %u).\n", m_playerId);
			printf("(QpangConnection::onWriteFinish) A time out error occurred, we gracefully ignore it.\n");
			printf("(QpangConnection::onWriteFinish) Corresponding time_out error message: %s.\n", errorMessage.c_str());
		}
		else if (errorMessage == "The semaphore timeout period has expired")
		{
			printf("(QpangConnection::onWriteFinish) An error occurred for player (id = %u).\n", m_playerId);
			printf("(QpangConnection::onWriteFinish) Received a semaphore timeout period, we gracefully ignore it.\n");
		}
		else
		{
			printf("(QpangConnection::onWriteFinish) An error occurred for player (id = %u).\n", m_playerId);
			printf("(QpangConnection::onWriteFinish) Received an unknown error, we should close the connection.\n");
			printf("(QpangConnection::onWriteFinish) Corresponding unknown error message: %s.\n", errorMessage.c_str());

			close();
		}
	}
}

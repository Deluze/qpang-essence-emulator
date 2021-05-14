#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <array>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "core/communication/packet/ServerPacket.h"
#include "core/communication/packet/QpangPacket.h"
#include "core/communication/encryption/BlowfishInstance.h"

using namespace boost::asio::ip;

class Player;
class QpangServer;
class PacketHandler;

class QpangConnection : public std::enable_shared_from_this<QpangConnection>
{
	using disconnect_handler = std::function<void(std::shared_ptr<QpangConnection>)>;
	using data_handler = std::function<void(std::shared_ptr<QpangConnection>, QpangPacket)>;

public:
	using Ptr = std::shared_ptr<QpangConnection>;

	QpangConnection(tcp::socket&& socket, uint32_t m_connectionId);

	void setDisconnectHandler(disconnect_handler closeCallback);
	void setDataHandler(data_handler dataCallback);

	void read();
	void send(const ServerPacket& packet);
	void close();
	uint32_t getId();

	void setIsInitialized();
	bool isInitialized();
	time_t getLastPacketReceived();

	std::array<char, 4> getKeyPart();

	void setPacketHandler(PacketHandler* packetHandler);

	void setPlayer(std::shared_ptr<Player> player);
	std::shared_ptr<Player> getPlayer();

	inline tcp::socket& getSocket()
	{
		return m_socket;
	}
private:

	void onHeaderRead(const boost::system::error_code& e);
	void onPayloadRead(const boost::system::error_code& e);

	void write(std::vector<char> buff);
	void onWriteFinish(const boost::system::error_code& e);

	tcp::socket m_socket;
	BlowfishInstance m_blowfishInstance;
	std::recursive_mutex m_receiveMutex;

	disconnect_handler m_closeCallback;
	data_handler m_dataCallback;

	bool m_isConnected;
	bool m_isInitialized;
	
	QpangPacket m_packet;
	PacketHandler* m_packetHandler;

	std::shared_ptr<Player> m_player;

	time_t m_timeLastPackReceived;
	uint32_t m_connectionId;
};
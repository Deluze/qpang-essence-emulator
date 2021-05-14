#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>

#include "core/communication/client/QpangConnection.h"

using namespace boost::asio::ip;

class QpangServer
{
	using accept_handler = std::function<void(QpangConnection::Ptr)>;

public:
	QpangServer(uint16_t port);

	void setAcceptHandler(accept_handler acceptCallback);
	void removeConnection(QpangConnection::Ptr connection);
	void listen();
	void accept();
private:

	const uint16_t m_port;

	bool m_isListening;
	accept_handler m_acceptCallback;
	boost::asio::io_service m_ioService;
	tcp::acceptor* m_acceptor;
	tcp::socket m_socket;
	uint32_t m_currConnectionId;

	std::unordered_map<uint32_t, QpangConnection::Ptr> m_connections;
};
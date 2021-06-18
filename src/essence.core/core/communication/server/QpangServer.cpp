#include "QpangServer.h"

QpangServer::QpangServer(uint16_t port) :
	m_port(port),
	m_isListening(false),
	m_socket(m_ioService)
{
	m_acceptor = new tcp::acceptor(m_ioService, tcp::endpoint(tcp::v4(), m_port));
}

void QpangServer::setAcceptHandler(accept_handler acceptCallback)
{
	m_acceptCallback = acceptCallback;
}

void QpangServer::listen()
{
	m_isListening = true;

	accept();

	try
	{
		m_ioService.run();
	}
	catch (const std::exception& e)
	{
		std::cout << "[QpangServer::listen]: " << e.what() << "\n";
	}
	catch (...)
	{
		std::cout << "[QpangServer::listen]: An exception occurred. \n";
	}
}

void QpangServer::removeConnection(QpangConnection::Ptr connection)
{
	//m_connections.erase(connection->getId());
}

void QpangServer::accept()
{
	if (!m_isListening)
		return;

	m_socket = tcp::socket(m_ioService);

	m_acceptor->async_accept(
		m_socket,
		[&](const boost::system::error_code& ec)
		{
			if (ec)
			{
				std::cout << ec.message() << std::endl;
				return;
			}

			m_currConnectionId++;

			QpangConnection::Ptr conn = std::make_shared<QpangConnection>(std::move(m_socket), m_currConnectionId);

			//TODO: Check this
			//m_connections[m_currConnectionId] = conn;

			if (m_acceptCallback)
				m_acceptCallback(conn);

			conn->read();

			accept();
		}
	);
}
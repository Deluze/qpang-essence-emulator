#include "Database.h"

#include <iostream>
#include <jdbc/cppconn/prepared_statement.h>

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

bool Database::initialize()
{
	driver = get_driver_instance();
	driver->threadInit();

	try
	{
		if (connect())
		{
			std::cout << "[Database::initialize]: Database connected.\n";

			con->setClientOption("OPT_RECONNECT", &m_reconnectState);
			con->setSchema(CONFIG_MANAGER->getString("DB_NAME").c_str());

			return true;
		}
	}
	catch (const sql::SQLException& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "[Database::initialize]: Failed to connect with the database. Please check the credentials.\n";

	driver->threadEnd();

	return false;
}

void Database::unlock()
{
	m_dbMx.unlock();
}

Statement::Ptr Database::prepare(const char* query, bool lock)
{
	if (lock)
		m_dbMx.lock();

	sql::PreparedStatement* stmt = con->prepareStatement(query);

	return std::make_shared<Statement>(stmt);
}

bool Database::connect()
{
	try
	{
		std::string host = CONFIG_MANAGER->getString("DB_HOST");
		uint32_t port = CONFIG_MANAGER->getInt("DB_PORT");
		std::string user = CONFIG_MANAGER->getString("DB_USER");
		std::string pass = CONFIG_MANAGER->getString("DB_PASS");

		std::string endpoint = std::string("tcp://") + host + std::string(":") + std::to_string(port);

		con = driver->connect(endpoint.c_str(), user.c_str(), pass.c_str());

		return true;
	}
	catch (const sql::SQLException& e)
	{
		std::cout << e.what() << std::endl;

		m_initialAttemptConnectCount++;
		
		if (m_initialAttemptConnectCount < 3)
			return connect();
	}

	return false;
}

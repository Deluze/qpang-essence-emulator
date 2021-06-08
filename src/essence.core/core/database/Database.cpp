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
			std::cout << "Database connected! \n";

			con->setClientOption("OPT_RECONNECT", &m_reconnectState);
			con->setSchema(CONFIG_MANAGER->getString("DB_NAME").c_str());

			return true;
		}
	}
	catch (const sql::SQLException& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "Failed to connect with the database. Please check your credentials \n";

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
		sql::SQLString host = "127.0.0.1";
		uint32_t port = 3306;
		sql::SQLString user = "root";
		sql::SQLString pass = "";

		sql::SQLString endpoint = std::string("tcp://") + host + std::string(":") + std::to_string(port);

		con = driver->connect(endpoint, user, pass);

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

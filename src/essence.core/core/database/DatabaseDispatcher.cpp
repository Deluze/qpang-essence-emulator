#include "DatabaseDispatcher.h"

#include <iostream>
#include <jdbc/cppconn/prepared_statement.h>

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

void DatabaseDispatcher::run()
{
	driver = get_driver_instance();

	connect();

	con->setClientOption("OPT_RECONNECT", &m_reconnectState);
	con->setSchema(CONFIG_MANAGER->getString("DB_NAME").c_str());

	m_isRunning = true;

	auto now = std::chrono::system_clock::now();
	auto nextQueryTime = now + std::chrono::minutes(1);

	while (m_isRunning)
	{
		m_dbMx.lock();
		const auto queries = m_queries;
		m_queries.clear();
		m_dbMx.unlock();

		for (const auto& [query, binds] : queries)
		{
			try
			{
				auto* stmt = con->prepareStatement(query);

				uint8_t index = 1;
				for (const auto& bind : binds)
				{
					if (!bind.isString) {
						if (bind.nullable)
							if (bind.intVal == 0)
								stmt->setNull(index, 0);
							else
								stmt->setBigInt(index, std::to_string(bind.intVal).c_str());
						else
							stmt->setBigInt(index, std::to_string(bind.intVal).c_str());
					}
					else {
						stmt->setString(index, std::string(bind.stringVal.begin(), bind.stringVal.end()));
					}

					index++;
				}

				stmt->execute();
				delete stmt;
			}
			catch (const std::exception& e)
			{
				std::cout << "[DatabaseDispatcher::run]: " << e.what() << '\n';
			}
		}

		now = std::chrono::system_clock::now();

		if (nextQueryTime < now) {
			std::cout << "[DatabaseDispatcher::run]: Keeping database dispatcher connection alive.\n";

			const auto statement = con->prepareStatement("SELECT * FROM qfighter.weapons LIMIT 0;");

			statement->execute();

			delete statement;

			nextQueryTime = now + std::chrono::minutes(1);
		}
	}
}

void DatabaseDispatcher::dispatch(const std::string& query, const std::vector<Bind>& binds)
{
	std::lock_guard<std::mutex> g(m_dbMx);

	m_queries.push_back({ query, binds });
}

void DatabaseDispatcher::executeAll()
{
	m_dbMx.lock();

	std::cout << "[DatabaseDispatcher::executeAll]: Started.";

	for (const auto& [query, binds] : m_queries)
	{
		try
		{
			auto* stmt = con->prepareStatement(query);

			uint8_t index = 1;
			for (const auto& bind : binds)
			{
				if (!bind.isString) {
					if (bind.nullable)
						if (bind.intVal == 0)
							stmt->setNull(index, 0);
						else
							stmt->setBigInt(index, std::to_string(bind.intVal).c_str());
					else
						stmt->setBigInt(index, std::to_string(bind.intVal).c_str());
				}
				else {
					stmt->setString(index, std::string(bind.stringVal.begin(), bind.stringVal.end()));
				}
				index++;
			}

			stmt->execute();
			delete stmt;
		}
		catch (const std::exception& e)
		{
			std::cout << "[DatabaseDispatcher::executeAll]: An exception occurred: " << e.what() << '\n';
		}
	}

	std::cout << "[DatabaseDispatcher::executeAll]: Done.\n";

	m_queries.clear();

	m_dbMx.unlock();
}

void DatabaseDispatcher::connect()
{
	try
	{
		std::string host = CONFIG_MANAGER->getString("DB_HOST");
		uint32_t port = CONFIG_MANAGER->getInt("DB_PORT");
		std::string user = CONFIG_MANAGER->getString("DB_USER");
		std::string pass = CONFIG_MANAGER->getString("DB_PASS");

		std::string endpoint = std::string("tcp://") + host + std::string(":") + std::to_string(port);

		con = driver->connect(endpoint.c_str(), user.c_str(), pass.c_str());
	}
	catch (const sql::SQLException& e)
	{
		std::cout << "[DatabaseDispatcher::connect]: An exception occurred: " << e.what() << std::endl;

		m_initialAttemptConnectCount++;

		if (m_initialAttemptConnectCount < 3)
			return connect();
	}
}

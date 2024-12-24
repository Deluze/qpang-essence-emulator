#pragma once

#include <cstdint>
#include <mutex>
#include <vector>

#include <jdbc/mysql_connection.h>

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/mysql_driver.h>

#include "core/database/Statement.h"

class DatabaseDispatcher
{
public:

	struct Bind
	{
		Bind(uint64_t val, bool nullable = false)
		{
			this->intVal = val;
			this->nullable = nullable;
		}

		Bind(std::u16string val)
		{
			this->stringVal = val;
			this->nullable = nullable;
			this->isString = true;
		}

		uint64_t intVal;
		std::u16string stringVal;

		bool isString = false;
		bool nullable = false;
	};

	void run();

	void dispatch(const std::string& query, const std::vector<Bind>& binds = {});

	void executeAll();
private:

	void connect();

	sql::Driver* driver = nullptr;
	sql::Connection* con = nullptr;

	bool m_reconnectState = true;
	bool m_isRunning = false;
	uint32_t m_reconnectCount = 0;
	uint8_t m_initialAttemptConnectCount = 0;

	std::vector<std::pair<std::string, std::vector<Bind>>> m_queries;

	std::mutex m_dbMx;
};
#pragma once

#include <cstdint>
#include <mutex>

#include <jdbc/mysql_connection.h>

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/mysql_driver.h>

#include "core/database/Statement.h"

class Database
{
public:
	bool initialize();

	Statement::Ptr prepare(const char* query, bool lock = true);

	void unlock();
private:
	bool connect();

	sql::Driver* driver = nullptr;
	sql::Connection* con = nullptr;

	bool m_reconnectState = true;
	uint32_t m_reconnectCount = 0;
	uint8_t m_initialAttemptConnectCount = 0;

	std::recursive_mutex m_dbMx;
};
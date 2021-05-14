#pragma once

#include <string>
#include <memory>
#include <cstdint>

#include <jdbc/cppconn/prepared_statement.h>

class StatementResult
{
public:
	using Ptr = std::shared_ptr<StatementResult>;

	StatementResult(sql::ResultSet* res);
	~StatementResult();

	bool hasNext();
	void next();
	bool hasResults();
	std::string getUTF8String(const char* col, size_t n = 253);
	std::u16string getString(const char* col, size_t n = 253);
	uint64_t getBigInt(const char* col);
	uint32_t getInt(const char* col);
	uint16_t getShort(const char* col);
	uint8_t getTiny(const char* col);
	float getFloat(const char* col);
	bool getFlag(const char* col);

private:
	sql::ResultSet* m_res;
	uint32_t m_currRow;
	uint32_t m_rowCount;
};
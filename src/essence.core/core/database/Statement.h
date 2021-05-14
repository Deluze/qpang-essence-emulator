#pragma once

#include <memory>

#include <jdbc/cppconn/prepared_statement.h>

#include "core/database/StatementResult.h"

class Statement
{
public:
	using Ptr = std::shared_ptr<Statement>;

	Statement(sql::PreparedStatement* stmt, bool lock = true);
	~Statement();

	void bindString(const char* val);
	void bindString(const std::u16string& val);
	void bindInteger(uint32_t val);

	void bindLong(uint64_t val);
	void bindLongOrNull(uint64_t val);

	void bindShort(uint16_t val);
	void bindByte(uint8_t val);
	void bindFlag(bool val);

	void clearBinds();

	StatementResult::Ptr fetch();
	uint32_t execute();
	uint64_t getLastInsertId();
private:
	sql::PreparedStatement* m_stmt;
	uint32_t m_bindIndex;
	uint64_t m_lastInsertId;

	bool m_lock;
};
#include "Statement.h"

#include "utils/StringConverter.h"

#include "core/Emulator.h"
#include "core/database/Database.h"

Statement::Statement(sql::PreparedStatement* stmt, bool lock) :
	m_stmt(stmt),
	m_bindIndex(1),
	m_lock(lock)
{

}

Statement::~Statement()
{
	if (m_lock)
		DATABASE->unlock();

	delete m_stmt;
}

void Statement::bindString(const char* val)
{
	m_stmt->setString(m_bindIndex, val);

	m_bindIndex++;
}

void Statement::bindString(const std::u16string& val)
{
	std::string string = StringConverter::Utf16ToUtf8(val);

	m_stmt->setString(m_bindIndex, string.c_str());

	m_bindIndex++;
}

void Statement::bindInteger(uint32_t val)
{
	m_stmt->setInt(m_bindIndex, val);

	m_bindIndex++;
}

void Statement::bindLong(uint64_t val)
{
	m_stmt->setBigInt(m_bindIndex, std::to_string(val).c_str());

	m_bindIndex++;
}

void Statement::bindLongOrNull(uint64_t val)
{
	if (val == NULL)
		m_stmt->setNull(m_bindIndex, 0);
	else
		return bindLong(val);

	m_bindIndex++;
}

void Statement::bindFlag(bool val)
{
	m_stmt->setBoolean(m_bindIndex, val);

	m_bindIndex++;
}

void Statement::bindDouble(const double_t val)
{
	m_stmt->setDouble(m_bindIndex, val);

	m_bindIndex++;
}

void Statement::clearBinds()
{
	m_bindIndex = 1;

	m_stmt->clearParameters();
}

StatementResult::Ptr Statement::fetch()
{
	sql::ResultSet* res = m_stmt->executeQuery();

	return std::make_shared<StatementResult>(res);
}

uint32_t Statement::execute()
{
	auto rows = m_stmt->executeUpdate();

	auto stmt = DATABASE->prepare("SELECT LAST_INSERT_ID() as id");
	auto res = stmt->fetch();

	m_lastInsertId = res->getBigInt("id");

	return rows;
}

uint64_t Statement::getLastInsertId()
{
	return m_lastInsertId;
}

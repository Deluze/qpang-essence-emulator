#include "StatementResult.h"

#include <istream>

#include "core/Emulator.h"
#include "core/database/Database.h"

#include "utils/StringConverter.h"

StatementResult::StatementResult(sql::ResultSet* res) :
	m_res(res),
	m_currRow(0)
{
	m_rowCount = res->rowsCount();

	if (m_rowCount > 0)
		m_res->next();
}

StatementResult::~StatementResult()
{
	delete m_res;
}

bool StatementResult::hasNext()
{
	return m_currRow < m_rowCount;
}

void StatementResult::next()
{
	m_currRow++;
	m_res->next();
}

bool StatementResult::hasResults()
{
	return m_rowCount > 0;
}

std::string StatementResult::getUTF8String(const char* col, size_t n)
{
	std::string str;

	try
	{
		std::istream* blob = m_res->getBlob(col);
		str = std::string(std::istreambuf_iterator<char>(*blob), {});
		delete blob;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	str.erase(std::find(str.begin(), str.end(), '\0'));

	return str;
}

std::u16string StatementResult::getString(const char* col, size_t n)
{
	std::string str;

	try
	{
		std::istream* blob = m_res->getBlob(col);
		str = std::string(std::istreambuf_iterator<char>(*blob), {});
		delete blob;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	str.erase(std::find(str.begin(), str.end(), '\0'));

	return StringConverter::Utf8ToUtf16(str);
}

uint64_t StatementResult::getBigInt(const char* col)
{
	return m_res->getUInt64(col);
}

uint32_t StatementResult::getInt(const char* col)
{
	return m_res->getUInt(col);
}

uint16_t StatementResult::getShort(const char* col)
{
	return static_cast<uint16_t>(m_res->getInt(col));
}

uint8_t StatementResult::getTiny(const char* col)
{
	return static_cast<uint8_t>(m_res->getInt(col));
}

float StatementResult::getFloat(const char* col)
{
	return static_cast<float>(m_res->getDouble(col));
}

bool StatementResult::getFlag(const char* col)
{
	return m_res->getBoolean(col);
}

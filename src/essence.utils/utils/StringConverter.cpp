#include "StringConverter.h"

#include <boost/locale/encoding_utf.hpp>

using boost::locale::conv::utf_to_utf;

std::string StringConverter::Utf16ToUtf8(const std::u16string& string)
{
	return utf_to_utf<char, char16_t>(string.c_str());
}

std::u16string StringConverter::Utf8ToUtf16(const std::string& string)
{
	return utf_to_utf<char16_t, char>(string.c_str());
}

std::u16string StringConverter::ToLowerCase(const std::u16string& string)
{
	std::u16string someString(string);
	std::transform(someString.begin(), someString.end(), someString.begin(), ::tolower);
	return someString;
}

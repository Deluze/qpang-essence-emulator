#pragma once

#include <iostream>
#include <string>
#include <algorithm>

class StringConverter {
public:
	static std::string Utf16ToUtf8(const std::u16string& string);
	static std::u16string Utf8ToUtf16(const std::string& string);
	static std::u16string ToLowerCase(const std::u16string& string);
};

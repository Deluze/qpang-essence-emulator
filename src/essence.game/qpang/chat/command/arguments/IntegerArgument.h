#pragma once

#include "qpang/chat/command/CommandArgument.h"

#include "utils/StringConverter.h"

class IntegerArgument : public CommandArgument
{
public:
	bool validate(const std::u16string& arg) override
	{
		auto string = StringConverter::Utf16ToUtf8(arg);

		for (const auto& possibleDigit : string)
			if (!std::isdigit(possibleDigit))
				return false;

		return true;
	}
};
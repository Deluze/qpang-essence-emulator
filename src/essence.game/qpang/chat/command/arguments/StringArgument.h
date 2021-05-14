#pragma once

#include "qpang/chat/command/CommandArgument.h"

class StringArgument : public CommandArgument
{
public:
	bool validate(const std::u16string& arg) override
	{
		return !arg.empty();
	}
};
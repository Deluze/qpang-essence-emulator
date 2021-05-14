#pragma once

#include <string>

class CommandArgument
{
public:
	virtual bool validate(const std::u16string& arg)
	{
		throw "CommandArgument::validate unset";
	}
};
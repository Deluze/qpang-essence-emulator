#pragma once

#include <string>
#include <cstdint>

struct Memo
{
	uint64_t id;
	uint32_t senderId;
	std::u16string nickname;
	std::u16string message;
	bool isOpened;
	time_t created;
};
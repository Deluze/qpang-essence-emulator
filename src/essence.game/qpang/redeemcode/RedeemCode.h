#pragma once

struct RedeemCode
{
	uint32_t id;
	uint64_t playerId;
	std::u16string redeemCode;
	uint32_t totalUses;
	uint32_t useCount;
	uint32_t cash;
	uint32_t don;
	bool isActive;
};
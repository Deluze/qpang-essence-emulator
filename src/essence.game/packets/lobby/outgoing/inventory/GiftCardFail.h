#pragma once

#include "packets/LobbyServerPacket.h"

class GiftCardFail : public LobbyServerPacket
{
public:
	enum LS_SEND_PRESENT_INDEC_FAIL
	{
		INVENTORY_FULL_UNSEAL = 362,
		SEND_GIFT_FAIL_TARGET_NOT_EXIST = 365,
		GIFT_FAIL_UNTRADABLE_CARD = 871
	};

	explicit GiftCardFail(const uint32_t code) : LobbyServerPacket(814) // 814 = LS_SEND_PRESENT_INDEC_FAIL
	{
		writeInt(code);
	}
};
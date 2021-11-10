#pragma once

#include "core/communication/packet/PacketEvent.h"

class EnchantItemEvent final : public PacketEvent
{
	// TODO: Move classes to their own file.

	class EnchantItemEventFailResponse : public LobbyServerPacket
	{
	public:
		enum LS_ENCHANT_ITEM_FAIL
		{
			LACK_OF_DON = 851,
			NOT_AVAILABLE_FOR_ENCHANT = 859
		};

		explicit EnchantItemEventFailResponse(const uint32_t code) : LobbyServerPacket(905)
		{
			writeInt(code);
		}
	};

	class EnchantItemEventSuccessResponse : public LobbyServerPacket
	{
	public:
		explicit EnchantItemEventSuccessResponse(const bool hasEnchantSucceeded, const uint32_t donBalance) : LobbyServerPacket(904) 
		{
			writeFlag(hasEnchantSucceeded);
			//writeInt(3); // 1 - Roi dances, 2 - Roi is sad
		}
	};

public:
	// Blue essence = handschoenen en schoenen
	// Yellow essence = Hoed, shirt en broek

	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		const auto cardId = packet.readLong();
		const auto cardItemId = packet.readInt();

		packet.readEmpty(4);

		const auto enchantCost = packet.readInt();

		const auto essenceCardId = packet.readLong();
		const auto essenceItemId = packet.readInt();

		// Golden spanner is for making sure an enchant can not downgrade.
		const auto goldenSpannerCardId = packet.readLong();
		const auto goldenSpannerItemId = packet.readInt();

		// TODO: Find out last 4 bytes.
		std::cout << "EnchantItemEvent::handle >> CardId: " << cardId <<
			", CardItemId: " << cardItemId <<
			", EnchantCost: " << enchantCost <<
			", EssenceCardId: " << essenceCardId <<
			", EssenceItemId: " << essenceItemId <<
			", GoldenSpannerCardId: " << goldenSpannerCardId <<
			", GoldenSpannerItemId: " << goldenSpannerItemId <<
			std::endl;

		conn->send(EnchantItemEventSuccessResponse(true, player->getDon()));
	}
};
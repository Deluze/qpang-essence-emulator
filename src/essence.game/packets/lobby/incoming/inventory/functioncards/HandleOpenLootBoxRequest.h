#pragma once

#include "core/communication/packet/PacketEvent.h"

class HandleOpenLootBoxRequest final : public PacketEvent
{
public:
	class SendOpenLootBoxResponse : public LobbyServerPacket
	{
	public:
		explicit SendOpenLootBoxResponse() : LobbyServerPacket(907)
		{
			// TODO: Implement response.
		}
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		// The lootbox to open.
		const auto lootBoxCardId = packet.readLong();
		const auto lootBoxItemId = packet.readInt();

		// ReSharper disable once CppUseStructuredBinding
		// ReSharper disable once CppTooWideScopeInitStatement
		const auto lootBoxCard = player->getInventoryManager()->get(lootBoxCardId);

		if (lootBoxCard.itemId != lootBoxItemId)
		{
			return;
		}

		// The key to open the lootbox with (can be Golden Spanner or Pangaea Key iirc).
		const auto keyCardId = packet.readLong();
		const auto keyItemId = packet.readInt();

		// ReSharper disable once CppTooWideScopeInitStatement
		// ReSharper disable once CppUseStructuredBinding
		const auto keyCard = player->getInventoryManager()->get(keyCardId);

		if (keyCard.itemId != keyItemId)
		{
			return;
		}

		// TODO: Remaining 8 bytes have yet to be figured out, if they're even of any importance.

		printf("[INCOMING HandleOpenLootBoxRequest] BoxCardId: %llu, BoxItemId: %u, KeyCardId: %llu, KeyItemId: %u\n",
			lootBoxCardId, lootBoxItemId, keyCardId, keyItemId);

		conn->send(SendOpenLootBoxResponse());
	}
};
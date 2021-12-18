#pragma once

#include "core/communication/packet/PacketEvent.h"

class HandleTradeAddCardRequest final : public PacketEvent
{
	class SendTradeAddCardSelf : public LobbyServerPacket
	{
	public:
		explicit SendTradeAddCardSelf(const uint32_t targetPlayerId) : LobbyServerPacket(889)
		{
			writeInt(targetPlayerId);
		}
	};

	class SendTradeAddCardOther : public LobbyServerPacket
	{
	public:
		explicit SendTradeAddCardOther(const uint32_t targetPlayerId, const uint8_t state, const uint32_t unk_03, 
										const std::array<char, 40> cardInfo) : LobbyServerPacket(891)
		{
			writeInt(targetPlayerId);
			writeInt(unk_03);
			writeByte(state);
			writeInt(unk_03);

			writeArray<char, 40>(cardInfo);
		}
	};


public:
	enum State {
		ADD_CARD = 100,
		REMOVE_CARD = 101,
		UNKNOWN = 102
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();
		const auto playerId = player->getId();

		const auto targetPlayerId = packet.readInt();
		const auto state = packet.readByte(); 

		const auto unk_03 = packet.readInt();

		// Contains cardId, itemId, these take up 12 bytes, so there's 32 bytes left of unknown info.
		const auto cardInfo = packet.readArray<char, 40>();

		const auto cardId = *(uint64_t*)(&cardInfo[0]);

		TradeManager* tradeManager = Game::instance()->getTradeManager();

		if (tradeManager->isTrading(playerId))
		{
			auto& tradeSessionInfo = tradeManager->getTradeSessionInfo(playerId);

			// If the client is trading with the player that the client thinks he is
			if (tradeSessionInfo.getBuddyId() == targetPlayerId)
			{
				if (tradeSessionInfo.isFinished())
				{
					// This guy just tried to to de trade bug, what a lovely person! https://www.youtube.com/watch?v=VuCp8gVZ2XU
					// Yeah, let's not do that.
					return;
				}

				if (state == ADD_CARD)
				{
					tradeSessionInfo.addCard(cardId);
				}
				else if (state == REMOVE_CARD)
				{
					if (!tradeSessionInfo.removeCard(cardId))
					{
						// The card wasn't proposed in the first place?
						// Let's not go through with this
						return;
					}
				}

				conn->send(SendTradeAddCardSelf(targetPlayerId));

				const auto targetPlayer = Game::instance()->getOnlinePlayer(targetPlayerId);
				if (targetPlayer != nullptr)
				{
					targetPlayer->send(SendTradeAddCardOther(playerId, state, unk_03, cardInfo));
				}
			}
		}
	}
};
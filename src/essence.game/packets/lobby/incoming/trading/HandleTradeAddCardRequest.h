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

		auto& tradeSessionInfo = tradeManager->getTradeSessionInfo(playerId);

		// TODO: Save added cards server side.

		conn->send(SendTradeAddCardSelf(targetPlayerId));

		const auto targetPlayer = Game::instance()->getOnlinePlayer(targetPlayerId);

		if (targetPlayer != nullptr)
		{
			targetPlayer->send(SendTradeAddCardOther(playerId, state, unk_03, cardInfo));
		}

		std::cout << "HandleTradeAddCardRequest::handle >> TargetPlayerId: " << targetPlayerId 
			<< ", State: " << (int)state 
			<< ", Unk03: " << unk_03 
			<< ", CardId: " << cardId
			<< std::endl;
	}
};
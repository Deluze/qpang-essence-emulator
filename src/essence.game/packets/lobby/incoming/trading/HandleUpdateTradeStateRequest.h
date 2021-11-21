#pragma once

#include "core/communication/packet/PacketEvent.h"

class HandleUpdateTradeStateRequest final : public PacketEvent
{
	class SendTradeCancelSelf : public LobbyServerPacket
	{
	public:
		explicit SendTradeCancelSelf(const uint32_t targetPlayerId, const uint8_t state) : LobbyServerPacket(885)
		{
			writeInt(targetPlayerId);
			writeByte(state);
		}
	};

	class SendTradeCancelOther : public LobbyServerPacket
	{
	public:
		explicit SendTradeCancelOther(const uint32_t targetPlayerId, const uint8_t state) : LobbyServerPacket(887)
		{
			writeInt(targetPlayerId);
			writeInt(targetPlayerId);
			writeByte(state);
		}
	};
public:
	enum TradeState {
		CANCEL_TRADE = 50,
		CONFIRM_AND_LOCK_TRADE = 51,
		ACCEPT_AND_FINISH_TRADE = 52
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();
		const auto playerId = player->getId();

		TradeManager* tradeManager = Game::instance()->getTradeManager();

		// NOTE: TargetPlayerId is unreliable so we grab the trading buddy id.
		const auto targetPlayerId = packet.readInt();
		const auto state = packet.readByte();

		const auto tradingBuddyId = Game::instance()->getTradeManager()->findTradingBuddyId(playerId);

		if (state == CANCEL_TRADE)
		{
			tradeManager->endTradeSession(playerId);
			tradeManager->endTradeSession(tradingBuddyId);

			// TODO: Move packet sends to the TradeManager class.
			conn->send(SendTradeCancelSelf(tradingBuddyId, state));

			const auto targetPlayer = Game::instance()->getOnlinePlayer(tradingBuddyId);

			if (targetPlayer != nullptr)
			{
				targetPlayer->send(SendTradeCancelOther(playerId, state));

				return;
			}
		}
	}
};
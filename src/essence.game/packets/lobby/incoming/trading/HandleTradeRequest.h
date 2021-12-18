#pragma once

#include "core/communication/packet/PacketEvent.h"
#include <packets/lobby/outgoing/trading/SendReceiveTradeRequestError.h>
#include <packets/lobby/outgoing/trading/SendDeliverTradeRequest.h>
#include <packets/lobby/outgoing/trading/SendTradeRequestSuccess.h>

class HandleTradeRequest final : public PacketEvent
{
public:
	enum TradeRequestErrorCode {
		ALREADY_TRADING = 380,
		DOES_NOT_WANT_TRADE_OFFERS = 877
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();
		const auto playerId = player->getId();

		TradeManager* tradeManager = Game::instance()->getTradeManager();

		// Check if the player that has sent the request is already trading, if so do not allow them to send a trade request to another player.
		if (tradeManager->isTrading(playerId))
		{
			return;
		}

		const auto targetPlayerId = packet.readInt();
		const auto targetPlayer = Game::instance()->getOnlinePlayer(targetPlayerId);

		// Check to see if the target player is still online.
		if (targetPlayer == nullptr)
		{
			return;
		}

		// Check if the targetted player is already trading, if they are, send an error response.
		if (tradeManager->isTrading(targetPlayerId))
		{
			conn->send(SendReceiveTradeRequestError(ALREADY_TRADING));

			return;
		}

		tradeManager->startTradeSession(playerId, targetPlayerId, true);

		conn->send(SendTradeRequestSuccess());
		
		//tradeManager->startTradeSession(targetPlayerId, playerId, true);
		targetPlayer->send(SendDeliverTradeRequest(playerId));
	}
};
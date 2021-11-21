#pragma once

#include "core/communication/packet/PacketEvent.h"
#include <packets/lobby/outgoing/trading/SendReceiveTradeRequestError.h>
#include <packets/lobby/outgoing/trading/SendDeliverTradeRequest.h>

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
			std::cout << "You are trading already." << std::endl;

			return;
		}

		const auto targetPlayerId = packet.readInt();
		const auto targetPlayer = Game::instance()->getOnlinePlayer(targetPlayerId);

		// Check to see if the target player is still online.
		if (targetPlayer == nullptr)
		{
			std::cout << "The target player is null." << std::endl;

			return;
		}

		// Check if the targetted player is already trading, if they are, send an error response.
		if (tradeManager->isTrading(targetPlayerId))
		{
			conn->send(SendReceiveTradeRequestError(ALREADY_TRADING));

			std::cout << "The target player is already trading." << std::endl;

			return;
		}

		// Save the pending 
		tradeManager->startTradeSession(playerId, targetPlayerId, true);

		// TODO: Move the sending of these packets.
		player->send(LobbyServerPacket(876));

		targetPlayer->send(SendDeliverTradeRequest(playerId));
	}
};
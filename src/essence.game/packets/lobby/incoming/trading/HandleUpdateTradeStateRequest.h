#pragma once

#include "core/communication/packet/PacketEvent.h"
#include <packets/lobby/outgoing/trading/SendTradeCancelSelf.h>
#include <packets/lobby/outgoing/trading/SendTradeCancelOther.h>
#include <packets/lobby/outgoing/trading/SendTradeFinished.h>
#include <packets/lobby/outgoing/trading/SendTradeRequestCancel.h>

class HandleUpdateTradeStateRequest final : public PacketEvent
{
public:
	enum State {
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

		// Are we trading (or is our trade pending)?
		if (tradeManager->isTrading(playerId, true))
		{
			auto& myTradeSession = tradeManager->getTradeSessionInfo(playerId);
			const auto tradingBuddyId = myTradeSession.getBuddyId();

			if (!myTradeSession.isPending())
			{
				// Is the target trading?
				if (tradeManager->isTrading(tradingBuddyId))
				{
					auto& targetTradeSession = tradeManager->getTradeSessionInfo(tradingBuddyId);

					const auto targetPlayer = Game::instance()->getOnlinePlayer(tradingBuddyId);
					if (targetPlayer != nullptr)
					{
						if (state == CANCEL_TRADE)
						{
							tradeManager->endTradeSession(playerId);
							tradeManager->endTradeSession(tradingBuddyId);

							conn->send(SendTradeCancelSelf(tradingBuddyId, state));
							targetPlayer->send(SendTradeCancelOther(playerId, state));
						}
						else if (state == CONFIRM_AND_LOCK_TRADE)
						{
							// Client pressed confirm, notify client and other client
							conn->send(SendTradeCancelSelf(tradingBuddyId, state));
							targetPlayer->send(SendTradeCancelOther(playerId, state));
						}
						else if (state == ACCEPT_AND_FINISH_TRADE)
						{
							if (!targetTradeSession.isFinished())
							{
								// The target player didn't finish yet, so mark this client finished
								// and then wait for the target player to also finish.
								myTradeSession.setFinished(true);
								return;
							}

							auto myProposedCards = myTradeSession.getProposedCards();
							auto targetProposedCards = targetTradeSession.getProposedCards();

							// The stored sessions are not important anymore
							tradeManager->endTradeSession(playerId);
							tradeManager->endTradeSession(tradingBuddyId);

							// Trade client cards to target player
							for (const auto cardId : myProposedCards)
							{
								if (!player->getInventoryManager()->hasCard(cardId))
								{
									// Pfff, this guy is beyond simple trade scamming.
									// He added a card to the trade he doesn't even have in his inventory.
									// Let's cancel this trade to not make the other player upset :)
									conn->send(SendTradeCancelSelf(tradingBuddyId, CANCEL_TRADE));
									targetPlayer->send(SendTradeCancelOther(playerId, CANCEL_TRADE));
									return;
								}

								auto card = player->getInventoryManager()->get(cardId);
								player->getInventoryManager()->tradeCard(card, targetPlayer);
							}

							// Trade target player cards to client
							for (const auto cardId : targetProposedCards)
							{
								if (!targetPlayer->getInventoryManager()->hasCard(cardId))
								{
									// Pfff, this guy is beyond simple trade scamming.
									// He added a card to the trade he doesn't even have in his inventory.
									// Let's cancel this trade to not make the other player upset :)
									conn->send(SendTradeCancelSelf(tradingBuddyId, CANCEL_TRADE));
									targetPlayer->send(SendTradeCancelOther(playerId, CANCEL_TRADE));
									return;
								}

								auto card = targetPlayer->getInventoryManager()->get(cardId);
								targetPlayer->getInventoryManager()->tradeCard(card, player);
							}

							// Update both players their inventories
							player->getInventoryManager()->sendCards();
							targetPlayer->getInventoryManager()->sendCards();

							// Notify both players that the trade has succesfully ended
							conn->send(SendTradeFinished(tradingBuddyId));
							targetPlayer->send(SendTradeFinished(playerId));
						}
					}
				}
			}
			else
			{
				// It's still pending
				if (state == CANCEL_TRADE)
				{
					tradeManager->endTradeSession(playerId);
					tradeManager->endTradeSession(tradingBuddyId);

					const auto targetPlayer = Game::instance()->getOnlinePlayer(tradingBuddyId);
					if (targetPlayer != nullptr)
					{
						targetPlayer->send(SendTradeRequestCancel());
					}
				}
			}
		}
	}
};
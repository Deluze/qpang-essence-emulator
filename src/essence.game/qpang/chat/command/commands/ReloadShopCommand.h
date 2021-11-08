#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "packets/lobby/outgoing/shop/ShopItems.h"

class ReloadShopCommand : public Command
{
public:
	ReloadShopCommand() : Command(3)
	{

	}

	void handle(const std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		Game::instance()->getShopManager()->initialize();
		Game::instance()->getShopManager()->sendShopItems(player);
		
		player->broadcast(u"Reloaded the shop.");
	}
};
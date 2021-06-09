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

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto shopManager = Game::instance()->getShopManager();
		shopManager->initialize();
		const auto items = Game::instance()->getShopManager()->list();
		
		Game::instance()->send(ShopItems(items));
		
		player->broadcast(u"Reloaded the shop.");
	}
};
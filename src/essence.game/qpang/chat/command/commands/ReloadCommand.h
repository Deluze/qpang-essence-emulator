#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

class ReloadCommand: public Command
{
public:
	ReloadCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{	
		Game::instance()->getShopManager()->initialize();
		Game::instance()->getWeaponManager()->initialize();
		Game::instance()->getSpawnManager()->initialize();
		Game::instance()->getCraneManager()->initialize();

		player->broadcast(u"Reloaded the shopmanager, weaponmanager, spawnmanager and cranemanager.");
	}
};
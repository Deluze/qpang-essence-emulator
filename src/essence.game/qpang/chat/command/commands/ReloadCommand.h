#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

class ReloadCommand final : public Command
{
public:
	ReloadCommand() : Command(3)
	{

	}

	void handle(const std::shared_ptr<Player> player, const std::vector<std::u16string>& args) override
	{	
		Game::instance()->getShopManager()->initialize();
		Game::instance()->getWeaponManager()->initialize();
		Game::instance()->getSpawnManager()->initialize();
		Game::instance()->getCraneManager()->initialize();
		Game::instance()->getPveManager()->initialize();

		player->broadcast(u"Reloaded the shopmanager, weaponmanager, spawnmanager, cranemanager and pve manager.");
	}
};
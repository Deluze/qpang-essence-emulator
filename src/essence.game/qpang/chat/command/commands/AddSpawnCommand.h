#pragma once

#include "qpang/chat/command/Command.h"

#include "qpang/room/RoomPlayer.h"
#include "qpang/room/tnl/net_events/server/gc_mesg.hpp"

class AddSpawnCommand final : public Command
{
public:
	int mCount;

	AddSpawnCommand() : Command(3)
	{
		mCount = 1;
	}

	void handle(const std::shared_ptr<Player> player, const std::vector<std::u16string>& args) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a room in order to use this command.");

			return;
		}

		const auto roomSessionPlayer= roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a game in order to use this command.");

			return; 
		}

		const int gameMode = roomPlayer->getRoom()->getMode();
		const int team = roomPlayer->getTeam();

		const int map = roomPlayer->getRoom()->getMap();

		// Player position
		const auto [x, y, z] = roomSessionPlayer->getPosition();

		const auto insertIntoPositionsStatement = 
			DATABASE->prepare("INSERT INTO `qfighter`.`positions` (`x`, `y`, `z`, `name`) VALUES (?, ?, ?, ?);");

		insertIntoPositionsStatement->bindDouble(x);
		insertIntoPositionsStatement->bindDouble(y);
		insertIntoPositionsStatement->bindDouble(z);

		const std::string name = "Malta Item ";
		const std::string fullName = std::string(name).append(std::to_string(mCount));

		insertIntoPositionsStatement->bindString(fullName.c_str());
		insertIntoPositionsStatement->execute();

		std::cout << "Added spawn point '" << fullName << "' at position x:" << x << ", y:" << y << ", z:" << z << "\n";

		mCount++;
	}
};
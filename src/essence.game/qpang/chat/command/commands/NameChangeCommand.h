#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

class NameChangeCommand : public Command
{
public:
	NameChangeCommand() : Command(3)
	{

	}

	std::vector<CommandArgument*> getArguments() override
	{
		return {
			Command::argTypes[Command::Validation::STRING],
			Command::argTypes[Command::Validation::STRING]
		};
	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		if (args.size() != 2) {
			player->broadcast(u"Usage: :nc <player> <nickname>");

			return;
		}

		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer != nullptr) {
			player->broadcast(u"You can only perform this command in the square.");

			return;
		}

		std::u16string targetPlayerNickName = args.at(0);
		auto targetPlayer = Game::instance()->getPlayer(targetPlayerNickName);

		if (targetPlayer == nullptr) {
			player->broadcast(u"This player does not exist.");

			return;
		}

		auto targetRoomPlayer = targetPlayer->getRoomPlayer();

		if (targetRoomPlayer != nullptr) {
			player->broadcast(u"This player is currently in a match or in a gameroom.");

			return;
		}

		std::u16string newNickName = args.at(1);

		if (targetPlayer->getName() == newNickName) {
			player->broadcast(u"The nickname can not be the same as the current one.");

			return;
		}

		if (newNickName.length() < 4 || newNickName.length() > 16) {
			player->broadcast(u"The length of the nickname must be between 4 and 16 characters.");

			return;
		}

		auto preparedStatement = DATABASE->prepare("SELECT * FROM players WHERE name = ? LIMIT 1");
		preparedStatement->bindString(std::string(newNickName.begin(), newNickName.end()).c_str());

		auto result = preparedStatement->fetch();

		if (result->hasResults()) {
			player->broadcast(u"A player with this nickname already exists.");

			return;
		}

		targetPlayer->setName(newNickName);

		player->broadcast(u"You have successfully changed the nickname.");

		Game::instance()->removeClient(targetPlayer);

		if (targetPlayer->isOnline()) {
			Game::instance()->getCacheManager()->getPlayerCacheManager()->invalidate(targetPlayer->getId());
			Game::instance()->getCacheManager()->getPlayerCacheManager()->cache(targetPlayer->getName());

			targetPlayer->close();
		}
	}
};
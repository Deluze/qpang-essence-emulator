#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"
#include "core/config/ConfigManager.h"
#include "utils/StringConverter.h"

class ConfigCommand : public Command
{
public:
	ConfigCommand() : Command(3)
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
			player->broadcast(u"Please enter a key and a value.");
			player->broadcast(u"Usage: :config <key> <value>");

			return;
		}

		const auto key = StringConverter::Utf16ToUtf8(args.at(0));
		const auto value = StringConverter::Utf16ToUtf8(args.at(1));
		
		CONFIG_MANAGER->set(key, value);

		player->broadcast(u"Updated the configuration.");
	}
};
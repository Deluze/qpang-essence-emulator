#include "CommandManager.h"

#include <boost/format.hpp>

#include "BanCommand.h"
#include "DatabaseCommand.h"
#include "ReloadCommand.h"
#include "DailyRewardCommand.h"
#include "UnlimitedCommand.h"
#include "qpang/chat/command/Command.h"
#include "qpang/chat/command/commands/NameChangeCommand.h"
#include "qpang/chat/command/commands/InvincibleCommand.h"
#include "qpang/chat/command/commands/PermBanCommand.h"
#include "qpang/chat/command/commands/DisconnectCommand.h"
#include "qpang/chat/command/commands/BroadcastCommand.h"
#include "qpang/chat/command/commands/DisconnectIDCommand.h"
#include "qpang/chat/command/commands/ForceStartCommand.h"
#include "qpang/chat/command/commands/ForceStartAllCommand.h"
#include "qpang/chat/command/commands/MuteCommand.h"
#include "qpang/chat/command/commands/UnmuteCommand.h"
#include "qpang/chat/command/commands/UpdateCommand.h"
#include "qpang/chat/command/commands/FinishCommand.h"
#include "qpang/chat/command/commands/LeaderboardCommand.h"
#include "qpang/chat/command/commands/PositionCommand.h"
#include "qpang/chat/command/commands/ReloadShopCommand.h"
#include "qpang/chat/command/commands/ConfigCommand.h"
#include "qpang/chat/command/commands/CloseCommand.h"
#include "qpang/chat/command/commands/SpectateCommand.h"

void CommandManager::initialize()
{
	m_commands = {
		{u"dc", new DisconnectCommand() },
		{u"dcid", new DisconnectIDCommand() },
		{u"bc", new BroadcastCommand() },
		{u"start", new ForceStartCommand() },
		{u"startall", new ForceStartAllCommand() },
		{u"mute", new MuteCommand() },
		{u"unmute", new UnmuteCommand() },
		{u"update", new UpdateCommand() },
		{u"finish", new FinishCommand() },
		{u"leaderboard", new LeaderboardCommand() },
		{u"pos", new PositionCommand() },
		{u"reloadshop", new ReloadShopCommand() },
		{u"config", new ConfigCommand() },
		{u"close", new CloseCommand() },
		{u"ban", new BanCommand()},
		{u"permban", new PermBanCommand()},
		{u"database", new DatabaseCommand()},
		//{u"unlimited", new UnlimitedCommand()},
		{u"reload", new ReloadCommand()},
		{u"spectate", new SpectateCommand()},
		{u"nc", new NameChangeCommand()},
		{u"invincible", new InvincibleCommand()}
	};
}

bool CommandManager::isCommand(const std::u16string& command)
{
	return m_commands.find(command) != m_commands.cend();
}

void CommandManager::handle(std::shared_ptr<Player> player, const std::u16string& command, const std::u16string& arg)
{
	auto it = m_commands.find(command);

	if (it == m_commands.cend())
		return;

	auto comm = (*it).second;

	if (!comm->canHandle(player))
		return player->broadcast(u"You do not have the permission to execute this command.");

	std::vector<std::u16string> args;

	if (!comm->getArguments().empty())
	{
		const auto validators = comm->getArguments();

		if (arg.empty())
			return player->broadcast(u"No arguments were passed with the given command.");

		if (validators.size() == 1)
		{
			if (!validators[0]->validate(arg))
				return player->broadcast(u"The first argument for the given command is invalid.");
		}

		size_t prevArg{ 0 };

		for (size_t i = 0; i < arg.size(); i++)
		{
			const auto character = arg.at(i);

			if (character == u' ' || i + 1 == arg.size())
			{
				if (validators.size() == args.size() + 1)
				{
					args.push_back(arg.substr(prevArg, arg.size()));
					break;
				}

				const auto newArg = arg.substr(prevArg, i);
				args.push_back(newArg);
				prevArg = i + 1;
			}
		}

		for (size_t i = 0; i < args.size(); i++)
		{
			if (!validators[i]->validate(args[i]))
				return player->broadcast(u"The arguments for the given command are invalid.");
		}
	}
	else
	{
		args.push_back(arg);
	}

	comm->handle(player, args);
}

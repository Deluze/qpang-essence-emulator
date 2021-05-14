#include "ChatManager.h"

#include "core/Emulator.h"
#include "core/database/Database.h"

void ChatManager::initialize()
{
	m_commandManager = new CommandManager();
	m_commandManager->initialize();
}

std::u16string ChatManager::chat(std::shared_ptr<Player> player, const std::u16string& message)
{
	auto mess = message;

	if (mess.empty())
		return mess;

	if (auto off = mess.find(u'\0'); off != -1)
		mess.erase(off, mess.size());

	if (mess.at(0) == u':')
	{
		auto off = mess.find(u' ');
		auto command = mess.substr(1, off != -1 ? off - 1 : mess.size());

		if (m_commandManager->isCommand(command))
		{
			m_commandManager->handle(player, command, message.substr(off == -1 ? command.size() + 1 : command.size() + 2));
			// if off != -1, a space has been given. Meaning we have to take the : + space into account
			// if off == -1, no space as been given. Meaning we only have to take the : into account

			return u"";
		}
	}

	mess = filter(mess);

	return mess;
}

std::u16string ChatManager::filter(const std::u16string& message)
{
	return message;
}

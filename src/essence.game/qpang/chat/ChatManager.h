#pragma once

#include <string>
#include <vector>

#include "core/database/Database.h"

#include "qpang/chat/command/CommandManager.h"

class Player;

class ChatManager
{
public:
	void initialize();
	std::u16string chat(std::shared_ptr<Player> player, const std::u16string& message);
	std::u16string filter(const std::u16string& message);

private:
	std::vector<std::u16string> m_wordFilter;

	CommandManager* m_commandManager;
};
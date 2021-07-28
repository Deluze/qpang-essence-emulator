#pragma once

#include <string>
#include <memory>
#include <unordered_map>

class Player;
class Command;

class CommandManager
{
public:
	void initialize();

	bool isCommand(std::u16string& command);
	void handle(std::shared_ptr<Player> player, const std::u16string& command, const std::u16string& arg);

private:

	std::unordered_map<std::u16string, Command*> m_commands;
};
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>


#include "qpang/player/Player.h"
#include "qpang/chat/command/CommandArgument.h"
#include "utils/StringConverter.h"

#include "qpang/chat/command/arguments/StringArgument.h"
#include "qpang/chat/command/arguments/IntegerArgument.h"

class Command
{
public:

	enum Validation : uint8_t
	{
		STRING,
		INTEGER,
	};

	Command(uint8_t rank) :
		m_rank(rank)
	{
	}

	virtual std::vector<CommandArgument*> getArguments()
	{
		return {};
	}

	virtual bool canHandle(std::shared_ptr<Player> player)
	{
		auto playerRank = player->getRank();

		if (m_rank == 3 && playerRank == 3) {
			return true;
		}

		if (m_rank == 4 && (playerRank == 3 || playerRank == 4)) {
			return true;
		}

		if (m_rank == 1 && (playerRank == 1 || playerRank == 5)) {
			return true;
		}

		return false;

		//// If the command rank is GM and the players rank is helper return false.
		//if (m_rank == 3 && playerRank == 4)
		//	return false;
		//// If the command rank is helper and the player rank is GM or helper return true.
		//else if (m_rank == 4 && (playerRank == 3 || playerRank == 4))
		//	return true;
		//// If the command rank is less than or equal to the players rank
		//else if (m_rank == 1 && (playerRank == 1 || playerRank == 5))
		//	return true;

		//return false;
	}

	uint32_t convertToInteger(const std::u16string& arg)
	{
		const auto string = StringConverter::Utf16ToUtf8(arg);

		return std::stoi(string);
	}

	virtual void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{

	}

	std::unordered_map<uint8_t, CommandArgument*> argTypes = {
		{ Command::Validation::STRING, new StringArgument() },
		{ Command::Validation::INTEGER, new IntegerArgument() },
	};

private:
	uint8_t m_rank;
};
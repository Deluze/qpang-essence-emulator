#pragma once

#include <unordered_map>
#include <cstdint>

#include "qpang/room/game_mode/modes/DeathMatch.h"
#include "qpang/room/game_mode/modes/TeamDeathMatch.h"
#include "qpang/room/game_mode/modes/ProtectTheEssence.h"
#include "qpang/room/game_mode/modes/Vip.h"

class GameMode;

class GameModeManager
{
public:
	GameMode* get(uint8_t mode)
	{
		auto it = m_gameModes.find(mode);

		if (it == m_gameModes.cend())
			return m_gameModes[1];

		return (*it).second;
	}
private:
	std::unordered_map<uint8_t, GameMode*> m_gameModes = {
		{1, new DeathMatch()},
		{2, new TeamDeathMatch()},
		{3, new ProtectTheEssence()},
		{4, new Vip()},
	};
};
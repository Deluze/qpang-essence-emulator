#pragma once

enum class SkillTarget : uint32_t
{
	SELF = 0,
	ENEMY_PLAYER = 1,
	TEAM_PLAYER = 2,
	ALL_ENEMY_PLAYERS = 3,
	ALL_TEAM_PLAYERS = 4
};
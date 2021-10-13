#include "RoomSkillManager.h"

#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"

void RoomSkillManager::initialize(std::shared_ptr<RoomSession> room)
{
	m_room = room;
	m_skillsForMode = Game::instance()->getSkillManager()->getSkillsForGameMode(room->getRoom()->getMode());
}

std::shared_ptr<Skill> RoomSkillManager::generateRandomSkill()
{
	// If there are no skills for this mode, then we can not generate a skill.
	if (m_skillsForMode.empty())
	{
		return nullptr;
	}

	// TODO: Better way to get a random skill.

	const auto randomIndex = rand() % m_skillsForMode.size();
	const auto &randomSkillCard = m_skillsForMode[randomIndex];

	return m_skillsForMode[randomIndex];
}

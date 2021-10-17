#include "RoomSessionSkillManager.h"

#include "Skill.h"

#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"

void RoomSessionSkillManager::initialize(std::shared_ptr<RoomSession> roomSession)
{
	m_roomSession = roomSession;
	m_skills = Game::instance()->getSkillManager()->getSkillsForGameMode(roomSession->getRoom()->getMode());
}

std::shared_ptr<Skill> RoomSessionSkillManager::generateRandomSkill()
{
	// If there are no skills for this mode, then we can not generate a skill.
	if (m_skills.size() == 0)
	{
		return nullptr;
	}

	const auto index = rand() % m_skills.size();

	return m_skills.at(index)();
}
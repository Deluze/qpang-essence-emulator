#include "RoomSkillManager.h"

#include "Skill.h"

#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"

void RoomSkillManager::initialize(std::shared_ptr<RoomSession> room)
{
	m_room = room;
	m_skills = Game::instance()->getSkillManager()->getSkillsForGameMode(room->getRoom()->getMode());
}

std::shared_ptr<Skill> RoomSkillManager::generateRandomSkill()
{
	// If there are no skills for this mode, then we can not generate a skill.
	if (m_skills.size() == 0)
	{
		return nullptr;
	}

	auto it = m_skills.begin();
	auto val = std::next(m_skills.begin(), rand() % m_skills.size());

	return (*val).second();
}

std::shared_ptr<Skill> RoomSkillManager::getSkillByItemId(uint32_t skillItemId)
{
	if (isValidSkillCard(skillItemId))
	{
		return (*m_skills.find(skillItemId)).second();
	}

	return nullptr;
}

bool RoomSkillManager::isValidSkillCard(uint32_t skillItemId)
{
	return m_skills.find(skillItemId) != m_skills.cend();
}

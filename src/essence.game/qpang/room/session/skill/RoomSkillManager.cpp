#include "RoomSkillManager.h"

#include "qpang/Game.h"
#include "qpang/skill/Skill.h"
#include "qpang/room/session/RoomSession.h"

void RoomSkillManager::initialize(std::shared_ptr<RoomSession> room)
{
	m_room = room;

	m_skills = Game::instance()->getSkillManager()->getSkillsForMode(room->getRoom()->getMode());
}

std::shared_ptr<Skill> RoomSkillManager::generateRandomSkill()
{
	//auto it = m_skills.begin();
	//
	//auto val = std::next(m_skills.begin(), rand() % m_skills.size());
	//
	//return (*val).second();

	return std::make_shared<Skill>();
}

bool RoomSkillManager::isSkillValid(uint32_t itemId)
{
	return m_skills.find(itemId) != m_skills.cend();
}

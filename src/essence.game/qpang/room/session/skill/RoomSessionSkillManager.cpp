#include "RoomSessionSkillManager.h"

#include "Skill.h"

#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"

void RoomSessionSkillManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
	m_skillsForGameMode = Game::instance()->getSkillManager()->getSkillsForGameMode(roomSession->getRoom()->getMode());
}

std::shared_ptr<Skill> RoomSessionSkillManager::getSkillByItemId(const uint32_t itemId)
{
	for (const auto& skillForGameMode : m_skillsForGameMode)
	{
		if (skillForGameMode()->getItemId() == itemId)
		{
			return skillForGameMode();
		}
	}

	return nullptr;
}

std::shared_ptr<Skill> RoomSessionSkillManager::generateRandomSkill()
{
	// If there are no skills for this mode, then we can not generate a skill.
	if (m_skillsForGameMode.empty())
	{
		return nullptr;
	}

	const auto index = rand() % m_skillsForGameMode.size();

	return m_skillsForGameMode.at(index)();
}

bool RoomSessionSkillManager::isValidSkillForGameMode(const uint32_t itemId)
{
	auto isValid = false;

	for (const auto &skill : m_skillsForGameMode)
	{
		if (const auto skillItemId = skill()->getItemId(); skillItemId == itemId)
		{
			isValid = true;

			break;
		}
	}

	return isValid;
}

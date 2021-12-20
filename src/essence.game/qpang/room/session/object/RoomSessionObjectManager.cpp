#include "RoomSessionObjectManager.h"

#include "Position.h"
#include "RoomSession.h"

#include <vector>

#include "gc_pve_object_init.hpp"

void RoomSessionObjectManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

uint32_t RoomSessionObjectManager::spawnObject(const Object object)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	const auto objectUid = (m_objects.size() + 1);
	m_objects[objectUid] = object;
	return objectUid;
}

void RoomSessionObjectManager::despawnObject(const uint32_t uid)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto object = findObjectByUid(uid);

	if (object == nullptr)
	{
		return;
	}

	roomSession->relayPlaying<GCPvEObjectInit>(0, uid, object->position.x, object->position.y, object->position.z, 0);

	m_objects.erase(uid);
}

RoomSessionObjectManager::Object* RoomSessionObjectManager::findObjectByUid(const uint32_t uid)
{
	const auto it = m_objects.find(uid);

	if (it == m_objects.end())
	{
		return nullptr;
	}

	return &it->second;
}

std::unordered_map<uint32_t, RoomSessionObjectManager::Object> RoomSessionObjectManager::getObjects()
{
	return m_objects;
}
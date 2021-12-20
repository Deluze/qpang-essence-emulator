#include "RoomSessionObjectManager.h"

#include "Position.h"
#include "RoomSession.h"
#include "RoomSessionPlayer.h"

#include <vector>

#include "gc_pve_object_init.hpp"

void RoomSessionObjectManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

uint32_t RoomSessionObjectManager::spawnObject(std::unique_ptr<PveObject> object)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	const auto objectUid = (m_objects.size() + 1);

	object->setUid(objectUid);
	roomSession->relayPlaying<GCPvEObjectInit>((U32)object->getType(), objectUid, object->getPosition().x, object->getPosition().y, object->getPosition().z, 0);

	m_objects[objectUid] = std::move(object);
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

	roomSession->relayPlaying<GCPvEObjectInit>(0, uid, object->getPosition().x, object->getPosition().y, object->getPosition().z, 0);

	m_objects.erase(uid);
}

std::unique_ptr<PveObject> RoomSessionObjectManager::findObjectByUid(const uint32_t uid)
{
	const auto it = m_objects.find(uid);

	if (it == m_objects.end())
	{
		return nullptr;
	}

	return std::move(it->second);
}

void RoomSessionObjectManager::onPlayerSync(std::shared_ptr<RoomSessionPlayer> session)
{
	for (auto& object : m_objects)
	{
		session->send<GCPvEObjectInit>((U32)object.second->getType(), object.first, object.second->getPosition().x, object.second->getPosition().y, object.second->getPosition().z, 0);
	}
}

void RoomSessionObjectManager::tick(std::shared_ptr<RoomSession> roomSession)
{
	for (auto& object : m_objects)
	{
		object.second->tick(roomSession);
	}
}
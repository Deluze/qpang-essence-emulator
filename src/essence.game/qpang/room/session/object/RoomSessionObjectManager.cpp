#include "RoomSessionObjectManager.h"

#include "gc_pve_destroy_object.hpp"
#include "gc_pve_door.hpp"
#include "gc_pve_object_init.hpp"

#include "Position.h"
#include "RoomSession.h"
#include "RoomSessionPlayer.h"

#include <qpang/room/session/object/objects/MoveableObject.h>

void RoomSessionObjectManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}


void RoomSessionObjectManager::tick() const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	for (const auto& [uid, object] : m_objects)
	{
		object->tick(roomSession);
	}
}

void RoomSessionObjectManager::initializeObjects()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_objects.clear();

	const auto mapId = roomSession->getPveRoundManager()->getMap();
	const auto objectData = Game::instance()->getPveManager()->getObjectDataByMapId(mapId);

	for (const auto& [uid, type, spawnPosition, endPosition, initialHealth, isMoveable, moveDuration, moveWait, linkedObjectId] : objectData)
	{
		m_objects[uid] = (isMoveable)
			? std::make_shared<MoveableObject>(uid, type, spawnPosition, endPosition, moveDuration, moveWait, linkedObjectId, initialHealth)
			: std::make_shared<PveObject>(uid, type, spawnPosition, linkedObjectId, initialHealth);

		roomSession->relayPlaying<GCPvEObjectInit>(static_cast<U32>(type), uid, spawnPosition.x, spawnPosition.y, spawnPosition.z, initialHealth);
	}
}

uint32_t RoomSessionObjectManager::spawnObject(const std::shared_ptr<PveObject>& object)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	const auto objectUid = (m_objects.size() + 1);

	roomSession->relayPlaying<GCPvEObjectInit>(static_cast<U32>(object->getType()), objectUid,
		object->getPosition().x, object->getPosition().y, object->getPosition().z,
		object->getInitialHealth());

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

	roomSession->relayPlaying<GCPvEObjectInit>(0, uid, object->getPosition().x, object->getPosition().y, object->getPosition().z, object->getHealth());

	m_objects.erase(uid);
}

void RoomSessionObjectManager::destroyObject(const uint32_t uid)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	if (const auto object = findObjectByUid(uid); object == nullptr)
	{
		return;
	}

	roomSession->relayPlaying<GCPvEDestroyObject>(uid);

	m_objects.erase(uid);
}

void RoomSessionObjectManager::removeAll()
{
	m_objects.clear();
}

void RoomSessionObjectManager::openGate(const uint32_t uid) const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	// TODO: Check if object uid exists on server side?

	roomSession->relayPlaying<GCPvEDoor>(uid, true);
}

void RoomSessionObjectManager::closeGate(const uint32_t uid) const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	// TODO: Check if object uid exists on server side?

	roomSession->relayPlaying<GCPvEDoor>(uid, false);
}

std::shared_ptr<PveObject> RoomSessionObjectManager::findObjectByUid(const uint32_t uid)
{
	const auto it = m_objects.find(uid);

	if (it == m_objects.end())
	{
		return nullptr;
	}

	return it->second;
}

void RoomSessionObjectManager::onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session)
{
	for (const auto& [uid, object] : m_objects)
	{
		session->send<GCPvEObjectInit>(static_cast<U32>(object->getType()), uid, object->getPosition().x, object->getPosition().y, object->getPosition().z, object->getHealth());
	}
}

void RoomSessionObjectManager::onStart()
{
	initializeObjects();
}

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

	initializeObjects();
}

void RoomSessionObjectManager::initializeObjects()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto statement = DATABASE->prepare(
		"SELECT "
		"pve_object_spawns.id as objectUid "
		",pve_object_spawns.type as objectType "
		",pve_object_spawns.spawn_position_x as spawnPositionX "
		",pve_object_spawns.spawn_position_y as spawnPositionY "
		",pve_object_spawns.spawn_position_z as spawnPositionZ "
		",pve_object_spawns.end_position_x as endPositionX "
		",pve_object_spawns.end_position_y as endPositionY "
		",pve_object_spawns.end_position_z as endPositionZ "
		",pve_object_spawns.is_moveable as isMoveable "
		",pve_object_spawns.move_duration as moveDuration "
		",pve_object_spawns.move_wait as moveWait "
		",pve_object_spawns.linked_object as linkedObject "
		"FROM pve_object_spawns "
		"INNER JOIN maps on maps.id = pve_object_spawns.map_id "
		"WHERE maps.map_id = ?;"
	);

	statement->bindInteger(roomSession->getRoom()->getMap());

	const auto result = statement->fetch();

	m_objects.clear();

	while (result->hasNext())
	{
		int objectUid = result->getInt("objectUid");
		bool isMoveable = result->getTiny("isMoveable");

		if (isMoveable)
		{
			m_objects[objectUid] = std::make_shared<MoveableObject>(
					objectUid,
					(eObjectType)result->getInt("objectType"),
					Position{
						result->getFloat("spawnPositionX"),
						result->getFloat("spawnPositionY"),
						result->getFloat("spawnPositionZ"),
					},
					Position{
						result->getFloat("endPositionX"),
						result->getFloat("endPositionY"),
						result->getFloat("endPositionZ"),
					},
					result->getInt("moveDuration"),
					result->getInt("moveWait"),
					result->getInt("linkedObject")
			);
		}
		else
		{
			m_objects[objectUid] = std::make_shared<PveObject>(
				objectUid,
				(eObjectType)result->getInt("objectType"),
				Position{
					result->getFloat("spawnPositionX"),
					result->getFloat("spawnPositionY"),
					result->getFloat("spawnPositionZ"),
				},
				result->getInt("linkedObject")
			);
		}

		result->next();
	}
}

uint32_t RoomSessionObjectManager::spawnObject(std::shared_ptr<PveObject> object)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	const auto objectUid = (m_objects.size() + 1);
	roomSession->relayPlaying<GCPvEObjectInit>(static_cast<U32>(object->getType()), objectUid, object->getPosition().x, object->getPosition().y, object->getPosition().z, 0);

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

	roomSession->relayPlaying<GCPvEObjectInit>(0, uid, object->getPosition().x, object->getPosition().y, object->getPosition().z, 0);

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
	for (const auto& object : m_objects)
	{
		session->send<GCPvEObjectInit>(static_cast<U32>(object.second->getType()), object.first, object.second->getPosition().x, object.second->getPosition().y, object.second->getPosition().z, 0);
	}
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

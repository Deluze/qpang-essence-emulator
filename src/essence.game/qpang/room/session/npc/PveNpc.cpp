#include "PveNpc.h"

#include "AABBHelper.h"
#include "gc_master_log.hpp"
#include "gc_pve_destroy_parts.hpp"
#include "gc_pve_die_npc.hpp"
#include "gc_pve_move_npc.hpp"
#include "gc_pve_npc_init.hpp"
#include "RandomHelper.h"
#include "RoomSession.h"
#include "TimeHelper.h"

std::mutex pathfindingMutex = {};

// TODO (Perhaps?):
// 
// Take this situation:
// |..|
// |..|
// |n.| <- this npc only has melee
// |nn| <- these have ranged guns, and are in range of the player
// |..|
// |..|
// |..|
// |..|
// |p.| <- this is the player
// |..|
//
// Problem:
// As the npcs with ranged guns are in range of the player, they will stop moving and instead attack.
// However, the melee npc is not in range, and thus wants to move, but it can't, because there's no path to the player.
// 
// Solution:
// Perhaps make a vector with cells that should be made free, and then check for each npc if their m_takenCell is in the vector.
// If it is, for that npc, return false in canAttackTargetPlayer, so that it will continue pathfinding.
// Once it's taken path isn't in the vector anymore, it can stop moving, and continue attacking.

std::function<void(uint32_t, uint32_t, const PathfinderCell&, const PathfinderCell&)> npcDoNextMove = [&](
	uint32_t roomId, uint32_t npcUid, const PathfinderCell& prevCell, const PathfinderCell& currCell)
{
	auto roomManager = Game::instance()->getRoomManager();
	if (!roomManager) // can this realistically even happen? I don't think so, unless the room manager gets destroyed, but it will only get destroyed if stuff is going havoc already.
		return;

	// Using the room manager mutex to be sure that the room is valid for as long as this code runs.
	// This also ensures that npcs/players will all still be there.
	std::lock_guard<std::recursive_mutex> lg(roomManager->m_roomMx);

	auto room = roomManager->get(roomId);
	if (!room)
		return;

	auto roomSession = room->getRoomSession();
	if (!roomSession)
		return;

	auto npcManager = roomSession->getNpcManager();
	if (!npcManager)
		return;
	
	auto npc = npcManager->findNpcByUid(npcUid);
	if (!npc)
		return;

	const auto pathFinder = npc->getPathFinder(roomSession);
	if (!pathFinder)
	{
		npc->clearPath();
		return;
	}

	npc->setPosition(pathFinder, prevCell);

	if (!npc->isNextMoveValid(roomSession, pathFinder, currCell))
	{
		npc->clearPath();
		return;
	}

	bool shouldStopMoving = npc->getIsMovingToPlayer() ?
		npc->canAttackTargetPlayer(roomSession, pathFinder) :
		npc->canAttackTargetPos(pathFinder);

	if (!shouldStopMoving)
	{
		npc->doPathfindingMove(roomSession, currCell);

		if (npc->didPathFinish())
		{
			npc->setPosition(pathFinder, currCell);
			npc->clearPath();
			return;
		}

		const auto nextCell = npc->getMoveCell();

		const float speed = npc->getSpeed();
		const float moveTime = pathFinder->calculateMoveTime(speed, currCell, nextCell);

		const int timeMilliseconds = std::round(moveTime * 1000.f);

		TimeHelper::setTimeOut<uint32_t, uint32_t, const PathfinderCell&, const PathfinderCell&>
			(timeMilliseconds, npcDoNextMove, roomId, npcUid, currCell, nextCell);
	}
	else
	{
		if (npc->getIsMovingToPlayer())
			npc->attackTargetPlayer(roomSession);
		else
			npc->attackTargetPos(roomSession);

		npc->clearPath();
	}
};

PveNpc::PveNpc(PveNpcData data, const PathfinderCell& spawnCell) :
	m_type(data.type),
	m_uid(data.uid),
	m_areaUid(data.areaUid),
	m_floorNumber(data.floorNumber),
	m_baseHealth(data.baseHealth),
	m_health(data.baseHealth),
	m_speed(data.speed),
	m_weaponItemId(data.weaponItemId),
	m_weaponBodyPartId(data.weaponBodyPartId),
	m_weaponType(data.weaponType),
	m_aiTime(data.aiTime),
	m_attackRange(data.attackRange),
	m_attackWidth(data.attackWidth),
	m_attackHeight(data.attackHeight),
	m_shouldRespawn(data.shouldRespawn),
	m_respawnTime(data.respawnTime),
	m_canDropLoot(data.canDropLoot),
	m_initialRotation(data.initialRotation),
	m_initialPosition(data.initialPosition),
	m_position(data.initialPosition),
	m_staticShootingPosition(data.staticShootingPosition),
	m_gradeType(data.gradeType),
	m_movementType(data.movementType),
	m_targetType(data.targetType),
	m_initialCell(spawnCell),
	m_takenCell(spawnCell),
	m_currentCell(spawnCell),
	m_lootDrops(std::move(data.lootDrops)),
	m_bodyParts(std::move(data.bodyParts))
{
}

PveNpc::PveNpc(PveNpcWaveData data, const PathfinderCell& spawnCell) :
	m_type(data.type),
	m_areaUid(0),
	m_floorNumber(0),
	m_baseHealth(data.baseHealth),
	m_health(data.baseHealth),
	m_speed(data.speed),
	m_weaponItemId(data.weaponItemId),
	m_weaponBodyPartId(data.weaponBodyPartId),
	m_weaponType(data.weaponType),
	m_aiTime(data.aiTime),
	m_attackRange(data.attackRange),
	m_attackWidth(data.attackWidth),
	m_attackHeight(data.attackHeight),
	m_canDropLoot(data.canDropLoot),
	m_initialRotation(0),
	m_initialPosition(data.initialPosition),
	m_position(data.initialPosition),
	m_staticShootingPosition({}),
	m_gradeType(data.gradeType),
	m_movementType(data.movementType),
	m_targetType(data.targetType),
	m_initialCell(spawnCell),
	m_takenCell(spawnCell),
	m_currentCell(spawnCell),
	m_lootDrops(std::move(data.lootDrops)),
	m_bodyParts(std::move(data.bodyParts))
{
}

void PveNpc::handleTargetStatic(const std::shared_ptr<RoomSession>& roomSession)
{
	if (!canShoot())
		return;

	const auto yCorrection = RandomHelper::getRandomFloat(-0.2f, 0.8f);
	const auto shootPos = Position{ m_staticShootingPosition.x, m_staticShootingPosition.y + yCorrection, m_staticShootingPosition.z };

	attack(roomSession, shootPos);
}

void PveNpc::improveTargetCell(Pathfinder* pathFinder)
{
	// Try to see if any of the cells 1 step closer to the npc are valid
	int xDelta = m_currentCell.x < m_targetCell.x ? -1 : 1;
	int zDelta = m_currentCell.z < m_targetCell.z ? -1 : 1;

	if (m_currentCell.x == m_targetCell.x)
		xDelta = 0;

	if (m_currentCell.z == m_targetCell.z)
		zDelta = 0;

	const auto tryNewCells = [&](bool closer) -> bool
	{
		// Only trying cell with new x
		{
			auto newTargetCell = m_targetCell;
			newTargetCell.x += closer ? xDelta : -xDelta;

			if (pathFinder->canPassThrough(newTargetCell.x, newTargetCell.z))
			{
				m_targetCell = newTargetCell;
				return true;
			}
		}

		// Only trying cell with new Z
		{
			auto newTargetCell = m_targetCell;
			newTargetCell.z += closer ? zDelta : -zDelta;

			if (pathFinder->canPassThrough(newTargetCell.x, newTargetCell.z))
			{
				m_targetCell = newTargetCell;
				return true;
			}
		}

		// Trying cell with new x and z
		{
			auto newTargetCell = m_targetCell;
			newTargetCell.x += closer ? xDelta : -xDelta;
			newTargetCell.z += closer ? zDelta : -zDelta;

			if (pathFinder->canPassThrough(newTargetCell.x, newTargetCell.z))
			{
				m_targetCell = newTargetCell;
				return true;
			}
		}

		return false;
	};

	// Let's first try the closer cells
	if (tryNewCells(true))
		return;

	// Now try the cells 1 step further away
	tryNewCells(false);
}

void PveNpc::startMovingToPos(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder, const Position& targetPos)
{
	m_targetCell = { pathFinder->getCellX(targetPos.x), pathFinder->getCellZ(targetPos.z) };

	// Try to find a closer cell if the target cell is in a wall
	if (!pathFinder->canPassThrough(m_targetCell.x, m_targetCell.z))
		improveTargetCell(pathFinder);

	if (pathFinder->solve(m_currentCell, m_targetCell, m_path) && m_path.size() > 1)
	{
		m_pathIdx = 1;

		// Starting at path[1], as the first pos is the current pos.
		TimeHelper::setTimeOut<uint32_t, uint32_t, const PathfinderCell&, const PathfinderCell&>
			(0, npcDoNextMove, roomSession->getRoom()->getId(), m_uid, m_currentCell, m_path[1]);
	}
}

void PveNpc::startMovingToPlayer(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	if (m_targetPlayerId == 0)
		return;

	auto targetPlayer = roomSession->find(m_targetPlayerId);
	if (!isPlayerValid(targetPlayer))
		return;

	m_isMovingToPlayer = true;
	startMovingToPos(roomSession, pathFinder, targetPlayer->getPosition());
}

bool PveNpc::canAttackTargetPos(Pathfinder* pathFinder)
{
	const auto targetCell = PathfinderCell{ pathFinder->getCellX(m_targetShootPosition.x), pathFinder->getCellZ(m_targetShootPosition.z) };

	const float distance = sqrtf(std::pow(targetCell.x - m_currentCell.x, 2) + std::pow(targetCell.z - m_currentCell.z, 2));
	if (distance > m_attackRange)
		return false;

	return pathFinder->lineOfSightBetween(m_currentCell, targetCell);
}

bool PveNpc::canAttackTargetPlayer(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	if (m_targetPlayerId == 0)
		return false;

	auto player = roomSession->find(m_targetPlayerId);
	if (!isPlayerValid(player))
		return false;

	m_targetShootPosition = player->getPosition();
	return canAttackTargetPos(pathFinder);
}

bool PveNpc::canShoot()
{
	const auto currentTime = time(nullptr);
	if ((m_lastAttackTime + (int)std::ceil(static_cast<float>(m_aiTime) / 1000.f)) > currentTime)
		return false;

	m_lastAttackTime = currentTime;
	return true;
}

void PveNpc::attackTargetPos(const std::shared_ptr<RoomSession>& roomSession)
{
	if (!canShoot())
		return;

	// We can attack!
	attack(roomSession, m_targetShootPosition);
}

void PveNpc::attackTargetPlayer(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_targetPlayerId == 0)
		return;

	auto player = roomSession->find(m_targetPlayerId);
	if (!player || !isPlayerValid(player))
		return;

	const auto [x, y, z] = player->getPosition();

	const auto yCorrection = RandomHelper::getRandomFloat(0.2f, 1.f);
	const auto targetPosition = Position{ x, (y + yCorrection), z };

	m_targetShootPosition = targetPosition;

	attackTargetPos(roomSession);
}

void PveNpc::handleTargetNear(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	if (!canAttackTargetPlayer(roomSession, pathFinder))
	{
		m_targetPlayerId = findClosestValidPlayerId(roomSession);

		if (m_movementType != eNpcMovementType::M_NONE)
			startMovingToPlayer(roomSession, pathFinder);

		return;
	}

	attackTargetPlayer(roomSession);
}

void PveNpc::handleTargetNearRevenge(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	if (!canAttackTargetPlayer(roomSession, pathFinder))
	{
		m_targetPlayerId = m_lastAttackerId;
		auto player = roomSession->find(m_targetPlayerId);
		if (!isPlayerValid(player))
			m_targetPlayerId = findClosestValidPlayerId(roomSession);

		if (m_movementType != eNpcMovementType::M_NONE)
			startMovingToPlayer(roomSession, pathFinder);

		return;
	}

	attackTargetPlayer(roomSession);
}

void PveNpc::handleTargetEssencePriority(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	// NOTE: Npcs will as first move walk in range to the essence, after that reconsider to attack essence, or walk & attack player.
	// This makes the stage a lot harder, perhaps don't let them do this.

	if (m_lastAttackerId != 0)
	{
		m_targetPlayerId = m_lastAttackerId;
		auto targetPlayer = roomSession->find(m_targetPlayerId);
		if (isPlayerValid(targetPlayer))
		{
			if (!canAttackTargetPlayer(roomSession, pathFinder))
			{
				// Perhaps we want to check to see if we can attack the target player
				// if it get's too easy this way.
				if (m_movementType != eNpcMovementType::M_NONE)
					startMovingToPlayer(roomSession, pathFinder);

				return;
			}

			attackTargetPlayer(roomSession);
			return;
		}
	}

	m_targetShootPosition = { 0, 1.1f, 0.f };
	if (!canAttackTargetPos(pathFinder))
	{
		m_isMovingToPlayer = false;

		if (m_movementType != eNpcMovementType::M_NONE)
			startMovingToPos(roomSession, pathFinder, m_targetShootPosition);

		return;
	}

	attackTargetPos(roomSession);
}

void PveNpc::handleLogic(const std::shared_ptr<RoomSession>& roomSession)
{
	const auto pathFinder = getPathFinder(roomSession);

	if (pathFinder && m_path.empty())
	{
		switch (m_targetType)
		{
		case eNpcTargetType::T_STATIC:
			handleTargetStatic(roomSession);
			break;
		case eNpcTargetType::T_NEAR:
			handleTargetNear(roomSession, pathFinder);
			break;
		case eNpcTargetType::T_NEAR_REVENGE:
			handleTargetNearRevenge(roomSession, pathFinder);
			break;
		case eNpcTargetType::T_ESSENCE_PRIORITY:
			handleTargetEssencePriority(roomSession, pathFinder);
			break;
		case eNpcTargetType::T_NONE: break;
		case eNpcTargetType::T_STATIC_REVENGE: break;
		case eNpcTargetType::T_DAMAGE: break;
		default:
			break;
		}
	}
}

void PveNpc::handleDeath(const std::shared_ptr<RoomSession>& roomSession)
{
	if (isDead() && m_shouldRespawn)
	{
		const auto currentTime = time(nullptr);

		if (m_timeOfDeath == NULL)
		{
			return;
		}

		if ((m_timeOfDeath + m_respawnTime) < currentTime)
		{
			// Reset time of death.
			m_timeOfDeath = NULL;

			roomSession->getNpcManager()->respawnNpcByUid(m_uid);
		}
	}
}

void PveNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	handleDeath(roomSession);

	if (isDead())
	{
		if ((m_takenCell.x != -1 && m_takenCell.z != -1) || (m_currentCell.x != -1 && m_currentCell.z != -1))
		{
			const auto pathFinder = getPathFinder(roomSession);
			if (pathFinder)
			{
				pathFinder->setCellTaken(m_takenCell, false);
				pathFinder->setCellTaken(m_currentCell, false);

				m_takenCell = { -1, -1 };
				m_currentCell = { -1, -1 };
			}
		}

		return;
	}

	handleLogic(roomSession);
}

uint32_t PveNpc::getLastAttackerId()
{
	return m_lastAttackerId;
}

void PveNpc::setLastAttackerId(const uint32_t id)
{
	m_lastAttackerId = id;
}

bool PveNpc::isNextMoveValid(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder, const PathfinderCell& cell)
{
	if (m_targetType == eNpcTargetType::T_ESSENCE_PRIORITY)
	{
		// We were moving to the essence, but we've now got a valid attacker player, so stop moving to the essence
		// and take care of the player >:)
		if (!m_isMovingToPlayer && m_lastAttackerId != 0)
		{
			auto targetPlayer = roomSession->find(m_lastAttackerId);
			if (isPlayerValid(targetPlayer))
				return false;
		}
	}

	auto targetPlayer = roomSession->find(m_targetPlayerId);
	if (m_isMovingToPlayer && !isPlayerValid(targetPlayer))
		return false;

	if (isDead())
		return false;

	auto currentPlayerCell = PathfinderCell
	{
		pathFinder->getCellX(m_isMovingToPlayer ? targetPlayer->getPosition().x : m_targetShootPosition.x),
		pathFinder->getCellZ(m_isMovingToPlayer ? targetPlayer->getPosition().z : m_targetShootPosition.x)
	};

	// If the cell is taken and it's not our taken cell
	if ((pathFinder->isCellTaken(cell) && m_takenCell != cell))
	{
		return false;
	}

	//if (m_targetCell != currentPlayerCell)
	//{
	//	// Player has moved at least 1 cell, reconsider logic
	//	m_path.clear();
	//	handleLogic(roomSession);
	// This would work, but would also impact server performance a bit probably
	// and this would also need a small timeout so that the npcs don't move too quickly.
	// This would however make them a lot smarter
	//}

	return true;
}

PathfinderCell PveNpc::getMoveCell()
{
	return m_path[m_pathIdx];
}

void PveNpc::clearPath()
{
	m_pathIdx = 0;
	m_path.clear();
}

bool PveNpc::didPathFinish() const
{
	return m_pathIdx >= m_path.size();
}

void PveNpc::doPathfindingMove(const std::shared_ptr<RoomSession> roomSession, const PathfinderCell& cell)
{
	roomSession->relayPlaying<GCPvEMoveNpc>(m_uid, static_cast<uint16_t>(cell.x), static_cast<uint16_t>(cell.z));

	m_pathIdx++;

	// ReSharper disable once CppTooWideScope
	const auto pathFinder = getPathFinder(roomSession);

	if (pathFinder)
	{
		pathFinder->setCellTaken(m_takenCell, false);

		m_takenCell = cell;

		pathFinder->setCellTaken(cell, true);
	}
}

void PveNpc::setPosition(Pathfinder* pathFinder, const PathfinderCell& cell)
{
	m_currentCell = cell;

	float x, z;

	pathFinder->cellToCoords(m_currentCell, x, z);

	m_position.x = x;
	m_position.z = z;
}

PathfinderCell PveNpc::getTargetCell()
{
	return m_targetCell;
}

bool PveNpc::isPlayerValid(const std::shared_ptr<RoomSessionPlayer>& player) const
{
	if (!player)
	{
		return false;
	}

	return !player->isDead() && !player->isInvincible() && player->getFloorNumber() == m_floorNumber;
}

uint32_t PveNpc::findClosestValidPlayerId(const std::shared_ptr<RoomSession>& roomSession) const
{
	uint32_t targetId = 0;
	float closestDistance = 40.f;

	for (const auto& player : roomSession->getPlayingPlayers())
	{
		if (!isPlayerValid(player) || !player->getPlayer())
		{
			continue;
		}

		const auto position = player->getPosition();
		const auto distance = AABBHelper::getDistanceBetweenPositions(m_position, position);

		if (distance < closestDistance)
		{
			closestDistance = distance;
			targetId = player->getPlayer()->getId();
		}
	}

	return targetId;
}

Pathfinder* PveNpc::getPathFinder(const std::shared_ptr<RoomSession>& roomSession) const
{
	// TODO: Perhaps check m_floorNumber instead
	return m_position.y < 0 ? roomSession->getUnderGroundPathfinder() :
		roomSession->getAboveGroundPathfinder();
}

void PveNpc::spawn(const std::shared_ptr<RoomSession>& roomSession) const
{
	roomSession->relayPlaying<GCPvENpcInit>(m_type, m_uid, m_initialPosition, m_initialRotation, m_health);
}

void PveNpc::spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const
{
	roomSessionPlayer->send<GCPvENpcInit>(m_type, m_uid, m_initialPosition, m_initialRotation, m_health);
}

void PveNpc::resetPosition()
{
	m_position = m_initialPosition;
	m_currentCell = m_initialCell;
	m_takenCell = m_initialCell;
}

void PveNpc::respawn(const std::shared_ptr<RoomSession>& roomSession)
{
	m_lastAttackTime = 0;

	resetPosition();

	clearPath();
	resetHealth();

	spawn(roomSession);
}

void PveNpc::die(const std::shared_ptr<RoomSession>& roomSession)
{
	roomSession->relayPlaying<GCPvEDieNpc>(m_uid);

	onDeath(roomSession);
}

void PveNpc::resetHealth()
{
	m_health = m_baseHealth;
}

uint16_t PveNpc::takeDamage(const uint16_t damage)
{
	if (damage > m_health)
	{
		m_health = 0;

		return (damage - m_health);
	}

	m_health = m_health - damage;

	return damage;
}

bool PveNpc::getIsMovingToPlayer()
{
	return m_isMovingToPlayer;
}

eNpcTargetType PveNpc::getTargetType()
{
	return m_targetType;
}

void PveNpc::attack(const std::shared_ptr<RoomSession>& roomSession, const Position targetPosition) const
{
	roomSession->relayPlaying<GCMasterLog>(m_uid, m_weaponBodyPartId, targetPosition, 0);
}

void PveNpc::onDeath(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_shouldRespawn)
	{
		m_timeOfDeath = time(nullptr);
	}

	if (m_canDropLoot)
	{
		dropLoot(roomSession);
	}
}

void PveNpc::setUid(const uint32_t uid)
{
	m_uid = uid;
}

uint32_t PveNpc::getUid() const
{
	return m_uid;
}

uint32_t PveNpc::getType() const
{
	return m_type;
}

Position PveNpc::getInitialPosition() const
{
	return m_initialPosition;
}

Position PveNpc::getPosition() const
{
	return m_position;
}

uint16_t PveNpc::getHealth() const
{
	return m_health;
}

uint16_t PveNpc::getBaseHealth() const
{
	return m_baseHealth;
}

float PveNpc::getSpeed() const
{
	return m_speed;
}

uint16_t PveNpc::getInitialRotation() const
{
	return m_initialRotation;
}

uint32_t PveNpc::getWeaponItemId() const
{
	return m_weaponItemId;
}

eWeaponType PveNpc::getWeaponType() const
{
	return m_weaponType;
}

uint32_t PveNpc::getAreaUid() const
{
	return m_areaUid;
}

bool PveNpc::shouldRespawn() const
{
	return m_shouldRespawn;
}

std::vector<NpcBodyPart> PveNpc::getBodyParts()
{
	return m_bodyParts;
}

bool PveNpc::isDead() const
{
	return m_health <= 0;
}

bool PveNpc::hasBodyPart(const uint32_t bodyPartId) const
{
	// ReSharper disable once CppUseStructuredBinding
	for (const auto& bodyPart : m_bodyParts)
	{
		if (bodyPart.id == bodyPartId)
		{
			return true;
		}
	}

	return false;
}

NpcBodyPart PveNpc::getBodyPartById(const uint32_t bodyPartId) const
{
	for (const auto& bodyPart : m_bodyParts)
	{
		if (bodyPart.id == bodyPartId)
		{
			return bodyPart;
		}
	}

	return {};
}

void PveNpc::dropLoot(const std::shared_ptr<RoomSession>& roomSession)
{
	std::vector<uint32_t> itemDrops{};

	for (const auto [itemId, probability] : m_lootDrops)
	{
		for (size_t i = 0; i < probability; i++)
		{
			itemDrops.push_back(itemId);
		}
	}

	const auto randomIndex = RandomHelper::getRandomNumber(0, itemDrops.size() - 1);
	const auto randomItemId = itemDrops[randomIndex];

	const bool isStageTwo = (roomSession->getPveRoundManager()->getCurrentRound() == eRound::CHESS_CASTLE_STAGE_2);

	if (randomItemId == static_cast<uint32_t>(eItemId::NONE))
	{
		m_canDropLoot = isStageTwo;

		return;
	}

	const auto randomPveItem = PveItem
	{
		randomItemId,
		m_position
	};

	roomSession->getPveItemManager()->spawnItem(std::make_shared<PveItem>(randomPveItem));

	m_canDropLoot = isStageTwo;
}

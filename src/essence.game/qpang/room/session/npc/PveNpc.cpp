#include "PveNpc.h"

#include "AABBHelper.h"
#include "gc_master_log.hpp"
#include "gc_pve_die_npc.hpp"
#include "gc_pve_move_npc.hpp"
#include "gc_pve_npc_init.hpp"
#include "RoomSession.h"
#include "TimeHelper.h"

std::mutex pathfindingMutex = {};

std::function<void(RoomSession::Ptr, PveNpc*, const PathfinderCell&, const PathfinderCell&)> npcDoNextMove = [&](
	const RoomSession::Ptr roomSession, PveNpc* npc, const PathfinderCell& prevCell, const PathfinderCell& currCell)
{
	std::lock_guard lg(pathfindingMutex);

	if (npc == nullptr)
		return;

	const auto pathFinder = npc->getPathFinder(roomSession);

	if (pathFinder == nullptr)
	{
		npc->clearPath();
		return;
	}

	npc->setPosition(pathFinder, prevCell);

	if (!npc->isNextMoveValid(pathFinder, currCell))
	{
		npc->clearPath();
		return;
	}

	if (!npc->canAttackTargetPlayer(pathFinder))
	{
		npc->doPathfindingMove(roomSession, currCell);
		if (npc->didPathFinish())
		{
			npc->clearPath();
			return;
		}

		const auto nextCell = npc->getMoveCell();

		const float speed = npc->getSpeed();
		const float moveTime = roomSession->getAboveGroundPathfinder()->calculateMoveTime(speed, currCell, nextCell);

		const int timeMilliseconds = std::round(moveTime * 1000.f);

		TimeHelper::setTimeOut<RoomSession::Ptr, PveNpc*, const PathfinderCell&, const PathfinderCell&>
			(timeMilliseconds, npcDoNextMove, roomSession, npc, currCell, nextCell);
	}
	else
	{
		npc->clearPath();
		npc->attackTargetPlayer(roomSession);
	}
};

PveNpc::PveNpc(PveNpcData data, const PathfinderCell& spawnCell) :
	m_type(data.type),
	m_areaUid(data.areaUid),
	m_floorNumber(data.floorNumber),
	m_baseHealth(data.baseHealth),
	m_health(data.baseHealth),
	m_speed(data.speed),
	m_weaponItemId(data.weaponItemId),
	m_weaponBodyPartId(data.weaponBodyPartId),
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

void PveNpc::handleNoMovement(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_targetType == eNpcTargetType::T_STATIC)
	{
		const auto currentTime = time(nullptr);
		if ((m_lastAttackTime + std::ceil((float)(m_aiTime) / 1000.f)) > currentTime)
			return;

		attack(roomSession, m_staticShootingPosition);

		m_lastAttackTime = currentTime;
	}
}

void PveNpc::startMovingToPlayer(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	if (m_targetPlayer != nullptr)
	{
		m_targetCell = { pathFinder->getCellX(m_targetPlayer->getPosition().x), pathFinder->getCellZ(m_targetPlayer->getPosition().z) };
		if (pathFinder->solve(m_currentCell, m_targetCell, m_path) && m_path.size() > 1)
		{
			m_pathIdx = 1;

			// Starting at path[1], as the first pos is the current pos.
			TimeHelper::setTimeOut<RoomSession::Ptr, PveNpc*, const PathfinderCell&, const PathfinderCell&>
				(0, npcDoNextMove, roomSession, this, m_currentCell, m_path[1]);
		}
	}
}

bool PveNpc::canAttackTargetPlayer(Pathfinder* pathFinder)
{
	if (!isPlayerValid(m_targetPlayer))
		return false;

	const auto playerPosition = m_targetPlayer->getPosition();
	const auto playerCell = PathfinderCell{ pathFinder->getCellX(playerPosition.x), pathFinder->getCellZ(playerPosition.z) };

	const float distance = sqrtf(std::pow(playerCell.x - m_currentCell.x, 2) + std::pow(playerCell.z - m_currentCell.z, 2));
	if (distance > m_attackRange)
		return false;

	return pathFinder->lineOfSightBetween(m_currentCell, playerCell);
}

void PveNpc::attackTargetPlayer(const std::shared_ptr<RoomSession>& roomSession)
{
	const auto currentTime = time(nullptr);
	if ((m_lastAttackTime + std::ceil((float)(m_aiTime) / 1000.f)) > currentTime)
		return;

	// We can attack!
	auto playerPos = m_targetPlayer->getPosition();
	playerPos.y += 1.f;

	attack(roomSession, playerPos);

	m_lastAttackTime = currentTime;
}

void PveNpc::handleTargetNear(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	if (!canAttackTargetPlayer(pathFinder))
	{
		m_targetPlayer = findClosestValidPlayer(roomSession);
		startMovingToPlayer(roomSession, pathFinder);
		return;
	}

	attackTargetPlayer(roomSession);
}

void PveNpc::handleTargetNearRevenge(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	if (!canAttackTargetPlayer(pathFinder))
	{
		m_targetPlayer = findValidAttackerPlayer(roomSession);
		if (!m_targetPlayer)
			m_targetPlayer = findClosestValidPlayer(roomSession);

		startMovingToPlayer(roomSession, pathFinder);
		return;
	}

	attackTargetPlayer(roomSession);
}

void PveNpc::handleMovement(const std::shared_ptr<RoomSession>& roomSession)
{
	const auto pathFinder = getPathFinder(roomSession);

	if (pathFinder && m_path.empty())
	{
		switch (m_targetType)
		{
		case eNpcTargetType::T_NEAR:
			handleTargetNear(roomSession, pathFinder);
			break;
		case eNpcTargetType::T_NEAR_REVENGE:
			handleTargetNearRevenge(roomSession, pathFinder);
			break;
		default: 
			break;
		}
	}
}

void PveNpc::handleDeath(const std::shared_ptr<RoomSession>& roomSession)
{
	// TODO: Check if the area has players in it, if it doesn't don't respawn the npc.
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
		return;
	}

	switch (m_movementType)
	{
	case eNpcMovementType::M_NONE:
		handleNoMovement(roomSession);
		break;
	case eNpcMovementType::M_PATH_FINDING:
		handleMovement(roomSession);
		break;
	default: 
		break;
	}
}

void PveNpc::setLastAttackerId(uint32_t id)
{
	m_lastAttackerId = id;
}

bool PveNpc::isNextMoveValid(Pathfinder* pathFinder, const PathfinderCell& cell)
{
	if (!isPlayerValid(m_targetPlayer) || isDead())
		return false;

	auto currentPlayerCell = PathfinderCell{ pathFinder->getCellX(m_targetPlayer->getPosition().x), pathFinder->getCellZ(m_targetPlayer->getPosition().z) };
	if (/*m_targetCell != currentPlayerCell ||*/ pathFinder->isCellTaken(cell))
		return false;

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

bool PveNpc::didPathFinish()
{
	return m_pathIdx >= m_path.size();
}

void PveNpc::doPathfindingMove(std::shared_ptr<RoomSession> roomSession, const PathfinderCell& cell)
{
	roomSession->relayPlaying<GCPvEMoveNpc>(m_uid, (uint16_t)cell.x, (uint16_t)cell.z);
	m_pathIdx++;

	auto pathFinder = getPathFinder(roomSession);
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

std::shared_ptr<RoomSessionPlayer> PveNpc::getTargetPlayer()
{
	return m_targetPlayer;
}

bool PveNpc::isPlayerValid(const std::shared_ptr<RoomSessionPlayer>& player) const
{
	if (!player)
	{
		return false;
	}

	return !player->isDead() && !player->isInvincible() && player->getFloorNumber() == m_floorNumber;
}

RoomSessionPlayer::Ptr PveNpc::findValidAttackerPlayer(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_lastAttackerId != 0)
	{
		auto player = roomSession->find(m_lastAttackerId);
		if (!isPlayerValid(player))
			return nullptr;

		return player;
	}

	return nullptr;
}

RoomSessionPlayer::Ptr PveNpc::findClosestValidPlayer(const std::shared_ptr<RoomSession>& roomSession) const
{
	RoomSessionPlayer::Ptr target = nullptr;
	float closestDistance = 40.f;

	for (const auto& player : roomSession->getPlayingPlayers())
	{
		if (!isPlayerValid(player))
			continue;

		const auto position = player->getPosition();
		const auto distance = AABBHelper::getDistanceBetweenPositions(m_position, position);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			target = player;
		}
	}

	return target;
}

Pathfinder* PveNpc::getPathFinder(const std::shared_ptr<RoomSession>& roomSession) const
{
	return m_position.y < 0 ? roomSession->getUnderGroundPathfinder() :
		roomSession->getAboveGroundPathfinder();
}

void PveNpc::spawn(const std::shared_ptr<RoomSession>& roomSession) const
{
	roomSession->relayPlaying<GCPvENpcInit>(m_type, m_uid, m_initialPosition, m_initialRotation);
}

void PveNpc::spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const
{
	roomSessionPlayer->send<GCPvENpcInit>(m_type, m_uid, m_initialPosition, m_initialRotation);
}

void PveNpc::resetPosition()
{
	m_position = m_initialPosition;
	m_currentCell = m_initialCell;
	m_takenCell = m_initialCell;
}

void PveNpc::respawn(const std::shared_ptr<RoomSession>& roomSession)
{
	const auto pathFinder = getPathFinder(roomSession);

	if (pathFinder)
	{
		pathFinder->setCellTaken(m_takenCell, false);
	}

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

bool PveNpc::hasBodyPart(const uint32_t bodyPartId)
{
	for (const auto& [id, health, weaponItemId, itemBoxId, isDualGun] : m_bodyParts)
	{
		if (id == bodyPartId)
		{
			return true;
		}
	}

	return false;
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

	// TODO: Properly create a random seed.
	// See: https://stackoverflow.com/questions/50662280/c-need-a-good-technique-for-seeding-rand-that-does-not-use-time
	srand(time(nullptr));

	const auto randomItemType = itemDrops[rand() % itemDrops.size()];

	const auto randomPveItem = PveItem
	{
		randomItemType,
		m_position
	};

	roomSession->getPveItemManager()->spawnItem(std::make_shared<PveItem>(randomPveItem));

	m_canDropLoot = false;
}

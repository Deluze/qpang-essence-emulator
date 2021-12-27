#include "PveNpc.h"

#include "AABBHelper.h"
#include "gc_master_log.hpp"
#include "gc_pve_die_npc.hpp"
#include "gc_pve_npc_init.hpp"
#include "RoomSession.h"

#include "TimeHelper.h"

#include <qpang/room/tnl/net_events/server/gc_pve_move_npc.hpp>

std::mutex pathfindingMutex = {};

std::function<void(RoomSession::Ptr, PveNpc*, const PathfinderCell&, const PathfinderCell&)> setNextMove = [&](
	RoomSession::Ptr roomSession, PveNpc* npc, const PathfinderCell& prevCell, const PathfinderCell& currCell)
{
	std::lock_guard lg(pathfindingMutex);

	if (npc == nullptr)
		return;

	auto pathFinder = npc->getPosition().y < 0 ? roomSession->getUnderGroundPathfinder() : roomSession->getAboveGroundPathfinder();
	if (pathFinder == nullptr)
	{
		npc->clearPath();
		return;
	}

	pathFinder->setCellTaken(prevCell, false);

	npc->setCurrentCell(roomSession, prevCell);

	float x, z;
	pathFinder->cellToCoords(prevCell, x, z);
	npc->setPosition(x, z);

	if (!npc->isNextMoveValid(pathFinder, currCell))
	{
		npc->clearPath();
		return;
	}

	// if we can't attack
	npc->doPathfindingMove(roomSession, currCell);
	pathFinder->setCellTaken(currCell, true);

	if (!npc->didPathFinish())
	{
		auto nextCell = npc->getMoveCell();

		// This needs to be npc speed
		const float speed = npc->getSpeed();
		float moveTime = roomSession->getAboveGroundPathfinder()->calculateMoveTime(speed, currCell, nextCell);

		int timeMilliseconds = std::round(moveTime * 1000.f);
		TimeHelper::setTimeOut<RoomSession::Ptr, PveNpc*, const PathfinderCell&, const PathfinderCell&>
			(timeMilliseconds, setNextMove, roomSession, npc, currCell, nextCell);
	}
	else
		npc->clearPath();
	// else
	// attack, clearpath and return
};

PveNpc::PveNpc(const uint32_t type, const uint16_t baseHealth, const float speed, const uint32_t weaponItemId, const uint8_t weaponBodyPartId,
	const uint32_t aiTime, const float attackWidth, const float attackHeight, const bool shouldRespawn, const uint32_t respawnTime,
	const bool canDropLoot, const uint16_t initialRotation, const Position initialPosition,
	const eNpcGradeType gradeType, const eNpcMovementType movementType, const eNpcTargetType targetType,
	std::vector<NpcLootDrop> lootDrops, std::vector<NpcBodyPart> bodyParts, const PathfinderCell& initialCell) :
	m_type(type),
	m_baseHealth(baseHealth),
	m_speed(speed),
	m_health(baseHealth),
	m_weaponItemId(weaponItemId),
	m_weaponBodyPartId(weaponBodyPartId),
	m_aiTime(aiTime),
	m_attackWidth(attackWidth),
	m_attackHeight(attackHeight),
	m_shouldRespawn(shouldRespawn),
	m_respawnTime(respawnTime),
	m_canDropLoot(canDropLoot),
	m_initialRotation(initialRotation),
	m_initialPosition(initialPosition),
	m_position(initialPosition),
	m_gradeType(gradeType),
	m_movementType(movementType),
	m_targetType(targetType),
	m_lootDrops(std::move(lootDrops)),
	m_bodyParts(std::move(bodyParts)),
	m_currentCell(initialCell),
	m_previousCell(initialCell),
	m_initialCell(initialCell)
{
}

void PveNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
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

	// Note: temporary piece of code to let npcs shoot a player when the player is in range.
	if (m_movementType == eNpcMovementType::M_NONE)
	{
		for (const auto& player : roomSession->getPlayingPlayers())
		{
			if (player->isDead())
			{
				continue;
			}

			// ReSharper disable once CppTooWideScopeInitStatement
			const auto distance = AABBHelper::getDistanceBetweenPositions(m_position, player->getPosition());

			if (distance < 7)
			{
				auto playerPosition = player->getPosition();

				constexpr auto yCorrection = 1.0f;
				playerPosition.y += yCorrection;

				attack(roomSession, playerPosition);
			}
		}
	}
	else if (m_movementType == eNpcMovementType::M_PATH_FINDING)
	{
		auto pathFinder = m_position.y < 0 ? roomSession->getUnderGroundPathfinder() :
			roomSession->getAboveGroundPathfinder();

		if (m_path.empty())
		{
			// if we can't attack
			if (m_targetType == eNpcTargetType::T_NEAR)
			{
				m_targetPlayer = findClosestValidPlayer(roomSession);
				if (m_targetPlayer != nullptr)
				{
					m_targetCell = { pathFinder->getCellX(m_targetPlayer->getPosition().x), pathFinder->getCellZ(m_targetPlayer->getPosition().z) };
					if (pathFinder->solve(m_currentCell, m_targetCell, m_path) && m_path.size() > 1)
					{
						m_pathIdx = 1;

						// Starting at path[1], as the first pos is the current pos.
						TimeHelper::setTimeOut<RoomSession::Ptr, PveNpc*, const PathfinderCell&, const PathfinderCell&>
							(0, setNextMove, roomSession, this, m_currentCell, m_path[1]);
					}
				}
			}
			// else, attack
		}
	}
}

bool PveNpc::isNextMoveValid(Pathfinder* pathFinder, const PathfinderCell& cell)
{
	if (m_targetPlayer == nullptr || m_targetPlayer->isDead() || m_targetPlayer->isInvincible() || isDead())
		return false;

	auto currentPlayerCell = PathfinderCell{ pathFinder->getCellX(m_targetPlayer->getPosition().x), pathFinder->getCellZ(m_targetPlayer->getPosition().z) };

	// The target position is outdated
	if (m_targetCell.x != currentPlayerCell.x && m_targetCell.z != currentPlayerCell.z || pathFinder->isCellTaken(cell))
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
}

void PveNpc::setPosition(float x, float z)
{
	m_position.x = x;
	m_position.z = z;
}

void PveNpc::setCurrentCell(std::shared_ptr<RoomSession> roomSession, const PathfinderCell& cell)
{
	/*auto pathFinder = m_position.y < 0 ? roomSession->getUnderGroundPathfinder() :
		roomSession->getAboveGroundPathfinder();

	pathFinder->setCellTaken(m_previousCell, false);
	pathFinder->setCellTaken(m_currentCell, true);*/

	m_previousCell = m_currentCell;
	m_currentCell = cell;
}

PathfinderCell PveNpc::getTargetCell()
{
	return m_targetCell;
}

std::shared_ptr<RoomSessionPlayer> PveNpc::getTargetPlayer()
{
	return m_targetPlayer;
}

RoomSessionPlayer::Ptr PveNpc::findClosestValidPlayer(const std::shared_ptr<RoomSession>& roomSession)
{
	RoomSessionPlayer::Ptr target = nullptr;
	float closestDistance = 40.f;

	for (const auto& player : roomSession->getPlayingPlayers())
	{
		if (player->isDead() || player->isInvincible())
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
	m_previousCell = m_initialCell;
}

void PveNpc::respawn(const std::shared_ptr<RoomSession>& roomSession)
{
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

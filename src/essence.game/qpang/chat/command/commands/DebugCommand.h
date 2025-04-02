#pragma once

#include "qpang/chat/command/Command.h"
#include "qpang/room/RoomPlayer.h"

#include "gc_shoot.hpp"

#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_npc_init.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_shoot_n2p.hpp>
#include <qpang/room/tnl/net_events/server/gc_master_log.hpp>
#include <qpang/room/tnl/net_events/client/cg_move_report.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_move_npc.hpp>

#include "TimeHelper.h"
#include "Maps.h"

inline std::vector<PathfinderCell> path = {};
inline int pathIdx = 0;

inline PathfinderCell npcCell = { 56, 51 };

// TODO: Eventually this function would be called something like "nextThink" and should move to the npc object.
// It should also decide after every step if it should stop moving and shoot, or continue moving.
inline std::function<void(RoomSession::Ptr, const PathfinderCell&, const PathfinderCell&)> nextMove = [&](RoomSession::Ptr roomSession,
	const PathfinderCell& prevCell, const PathfinderCell& currCell)
{
	npcCell = prevCell;

	roomSession->relayPlaying<GCPvEMoveNpc>(50, (uint16_t)currCell.x, (uint16_t)currCell.z);
	pathIdx++;

	if (pathIdx < path.size())
	{
		auto nextCell = path[pathIdx];

		const float speed = 5.f;
		float moveTime = roomSession->getAboveGroundPathfinder()->calculateMoveTime(speed, currCell, nextCell);

		int timeMilliseconds = std::round(moveTime * 1000.f);
		TimeHelper::setTimeOut<RoomSession::Ptr, const PathfinderCell&, const PathfinderCell&>
			(timeMilliseconds, nextMove, roomSession, currCell, nextCell);
	}
};

class DebugCommand final : public Command
{
public:
	DebugCommand() : Command(3)
	{
	}

	std::vector<CommandArgument*> getArguments() override
	{
		return {
			Command::argTypes[Command::Validation::INTEGER]
		};
	}

	void handle(const std::shared_ptr<Player> player, const std::vector<std::u16string>& args) override
	{
		const auto roomPlayer = player->getRoomPlayer();
		if (roomPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a room in order to use this command.");
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();
		if (roomSessionPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a game in order to use this command.");
			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		auto a1 = convertToInteger(args[0]);
		if (a1 == 0)
			Maps::recordMoves = true;
		else if (a1 == 1)
			Maps::recordMoves = false;
		else if (a1 == 2)
		{
			std::cout << "inline std::vector<std::vector<uint8_t>> debugWorldLayout = {" << std::endl;

			for (int x = 0; x < Maps::debugWorldLayout.size(); ++x)
			{
				std::cout << "	{ ";

				for (int z = 0; z < Maps::debugWorldLayout[0].size(); ++z)
				{
					if (z == Maps::debugWorldLayout[0].size() - 1)
						std::cout << (int)Maps::debugWorldLayout[x][z];
					else
						std::cout << (int)Maps::debugWorldLayout[x][z] << ", ";
				}

				if (x == Maps::debugWorldLayout.size() - 1)
					std::cout << " }" << std::endl;
				else
					std::cout << " }," << std::endl;
			}

			std::cout << "};";
		}
		else if (a1 == 3)
		{
			auto pathFinder = roomSession->getAboveGroundPathfinder();

			PathfinderCell targetCell = {	pathFinder->getCellX(roomSessionPlayer->getPosition().x), 
											pathFinder->getCellZ(roomSessionPlayer->getPosition().z)	};

			if (pathFinder->solve(npcCell, targetCell, path) && path.size() > 1)
			{
				pathIdx = 1;

				// Starting at path[1], as the first pos is the current pos.
				TimeHelper::setTimeOut<RoomSession::Ptr, const PathfinderCell&, const PathfinderCell&>
					(0, nextMove, roomSession, npcCell, path[1]);
			}
		}
	}
};
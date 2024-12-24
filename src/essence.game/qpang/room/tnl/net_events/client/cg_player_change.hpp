#ifndef CG_PLAYER_CHANGE_H
#define CG_PLAYER_CHANGE_H

#include "GameNetEvent.h"
#include "gc_player_change.hpp"

class CGPlayerChange : public GameNetEvent
{
	typedef NetEvent Parent;


public:

	enum CMD {
		CHARACTER = 1,
		TEAM = 2,
	};

	CGPlayerChange() : GameNetEvent{ CG_PLAYER_CHANGE, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
		bstream->read(&value);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (cmd == CHARACTER)
		{
			if (value != 333 && value != 343 && value != 578 && value != 579 && value != 850 && value != 851)
			{
				return;
			}

			player->setCharacter(value);

			if (const auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
			{
				roomPlayer->getRoom()->broadcastWaiting<GCPlayerChange>(player, cmd, value);
			}
		}
		else if (cmd == TEAM)
		{
			if (const auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
			{
				if (roomPlayer->isPlaying())
				{
					return;
				}

				if (const auto room = roomPlayer->getRoom(); room->isTeamAvailable(value) && room->getMode() != GameMode::PVE)
				{
					roomPlayer->setTeam(value);
				}
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGPlayerChange>(ps);
	};

	U32 playerId;
	U8 cmd;
	U32 value;

	TNL_DECLARE_CLASS(CGPlayerChange);
};
#endif
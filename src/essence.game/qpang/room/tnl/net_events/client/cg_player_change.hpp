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
		if (cmd == CMD::CHARACTER)
		{
			if (value != 333 && value != 343 && value != 578 && value != 579 && value != 850 && value != 851)
				return;

			player->setCharacter(value);

			if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
				roomPlayer->getRoom()->broadcastWaiting<GCPlayerChange>(player, cmd, value);
		}
		else if (cmd == CMD::TEAM)
		{
			if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
			{
				if (roomPlayer->isPlaying())
					return;

				auto room = roomPlayer->getRoom();

				if (room->isTeamAvailable(value))
					roomPlayer->setTeam(value);
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
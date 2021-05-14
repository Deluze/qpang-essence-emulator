#ifndef CG_READY_H
#define CG_READY_H

#include "GameNetEvent.h"
#include "gc_ready.hpp"

#include "qpang/room/session/RoomSession.h"

class CGReady : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum CMD : U32
	{
		UNREADY = 0,
		READY = 1,
	};

	CGReady() : GameNetEvent{ CG_READY, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (roomPlayer->isPlaying())
				return;

			if (cmd == CMD::UNREADY)
				roomPlayer->setReady(false);

			else if (cmd == CMD::READY)
			{
				auto room = roomPlayer->getRoom();

				if (room->isPlaying())
				{
					if (!room->canStartInTeam(roomPlayer->getTeam()))
						return player->broadcast(u"There are too many players on this team! Consider switching teams.");
					
					if (auto session = room->getRoomSession(); session != nullptr)
						if (session->isAlmostFinished())
							return player->broadcast(u"Please wait, this match is almost over!");

					roomPlayer->setReady(true);
					roomPlayer->setPlaying(true);
					conn->startLoading(room, roomPlayer);
					roomPlayer->onStart();
				}
				else
					roomPlayer->setReady(true);
			}
		}
	}
	
	void process(EventConnection* ps)
	{
		post<CGReady>(ps);
	};

	U32 playerId;
	U32 cmd;

	TNL_DECLARE_CLASS(CGReady);
};
#endif
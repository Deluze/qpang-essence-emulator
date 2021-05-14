#ifndef CG_EXIT_H
#define CG_EXIT_H

#include "GameNetEvent.h"

class CGExit : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum Command : U8
	{
		LEAVE = 0,
		KICK = 1,
		MASTER_KICK_IDLE = 2,
	};

	CGExit() : GameNetEvent{ CG_EXIT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};
	CGExit(U32 playerId, U8 cmd)
	{
		this->playerId = playerId;
		this->cmd = cmd;
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(playerId);
		bstream->write(cmd);
	};

	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
			return;

		auto room = roomPlayer->getRoom();

		if (cmd == Command::LEAVE)
			room->removePlayer(player->getId());
		else if (cmd == Command::KICK)
		{
			if (player->getId() != room->getMasterId())
				return;

			if (auto target = room->getPlayer(playerId); target != nullptr)
				room->removePlayer(target->getPlayer()->getId());
		}
	}

	void process(EventConnection* ps)
	{
		post<CGExit>(ps);
	};

	U32 playerId;
	U8 cmd;

	TNL_DECLARE_CLASS(CGExit);
};
#endif
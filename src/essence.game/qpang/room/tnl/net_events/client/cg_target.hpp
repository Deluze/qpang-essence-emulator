#ifndef CG_TARGET_H
#define CG_TARGET_H

#include "GameNetEvent.h"
#include "gc_target.hpp"

class CGTarget : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGTarget() : GameNetEvent{ CG_TARGET, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&cmd);
		bstream->read(&playerId);
		bstream->read(&targetId);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		std::cout << "CGTarget::handle >> CMD: " << cmd << ", PlayerID: " << playerId << ", TargetID: " << targetId << std::endl;

		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		const auto roomSession = roomPlayer->getRoom()->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		roomSession->relayPlaying<GCTarget>(cmd, playerId, targetId);
	};

	void process(EventConnection* ps)
	{
		post<CGTarget>(ps);
	};

	U32 playerId;
	U32 cmd;
	U32 targetId;

	TNL_DECLARE_CLASS(CGTarget);
};
#endif
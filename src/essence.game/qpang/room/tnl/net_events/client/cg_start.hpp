#ifndef CG_START_H
#define CG_START_H

#include "GameNetEvent.h"
#include "gc_start.hpp"

class CGStart : public GameNetEvent
{
	typedef NetEvent Parent;

public:
	U32 playerId; //88
	U8 unk_01; //92 // boolean?

	CGStart() : GameNetEvent{ CG_START, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&unk_01);

		//std::cout << "CGStart::unpack >> PlayerId: " << playerId << ", unk_01: " << unk_01 << std::endl;
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (player->getId() != roomPlayer->getRoom()->getMasterId())
				return;

			roomPlayer->getRoom()->start();
		}
	}

	void process(EventConnection* ps)
	{
		post<CGStart>(ps);
	};

	TNL_DECLARE_CLASS(CGStart);
};
#endif
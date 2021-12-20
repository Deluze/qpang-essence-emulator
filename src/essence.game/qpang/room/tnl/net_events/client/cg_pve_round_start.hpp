#ifndef CG_PVE_ROUND_START_H
#define CG_PVE_ROUND_START_H

#include "GameNetEvent.h"

class CGPvERoundStart : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvERoundStart() : GameNetEvent{ CG_PVE_ROUND_START, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}
	void unpack(EventConnection* conn, BitStream* bstream) override {}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		if (roomPlayer->isSpectating())
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		roomSession->getPveRoundManager()->onStartNewRound();
	}

	void process(EventConnection* ps) override
	{
		post<CGPvERoundStart>(ps);
	}

	TNL_DECLARE_CLASS(CGPvERoundStart);
};
#endif
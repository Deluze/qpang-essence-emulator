#ifndef CG_PVE_AREA_TRIGGER_H
#define CG_PVE_AREA_TRIGGER_H

#include "GameNetEvent.h"

class CGPvEAreaTrigger final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 areaId;

	CGPvEAreaTrigger() : GameNetEvent{ CG_PVE_AREA_TRIGGER, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&areaId);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
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

		//roomSession->getPveAreaManager()->onAreaTrigger(areaId, player->getId());
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEAreaTrigger>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEAreaTrigger);
};
#endif
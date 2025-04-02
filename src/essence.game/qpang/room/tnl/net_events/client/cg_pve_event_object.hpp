#ifndef CG_PVE_EVENT_OBJECT_H
#define CG_PVE_EVENT_OBJECT_H

#include "GameNetEvent.h"
#include "gc_pve_event_object.hpp"

class CGPvEEventObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid;
	bool flag; // should move?

	CGPvEEventObject() : GameNetEvent { CG_PVE_EVENT_OBJECT, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&objectUid);
		flag = bstream->readFlag();
	};

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

		if (roomSessionPlayer->isDead())
		{
			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		if (const auto objectPtr = roomSession->getObjectManager()->findObjectByUid(objectUid); objectPtr != nullptr)
			objectPtr->onEvent(roomSession);
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEEventObject>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEEventObject);
};
#endif
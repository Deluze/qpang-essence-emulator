#ifndef CG_PVE_EVENT_OBJECT_H
#define CG_PVE_EVENT_OBJECT_H

#include "GameNetEvent.h"
#include "gc_pve_event_object.hpp"

class CGPvEEventObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 objectUid;
	bool flag;

	CGPvEEventObject() : GameNetEvent { CG_PVE_EVENT_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};

	void unpack(EventConnection* conn, BitStream* bstream) 
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

		//auto objectPtr = roomSession->getObjectManager()->findObjectByUid(objectUid);
		//if (objectPtr)
		{
			//objectPtr->onEvent(roomSession);
			roomSession->relayPlaying<GCPvEEventObject>(objectUid, flag);
		}
	}

	void process(EventConnection* ps) 
	{
		post<CGPvEEventObject>(ps);
	};

	TNL_DECLARE_CLASS(CGPvEEventObject);
};
#endif
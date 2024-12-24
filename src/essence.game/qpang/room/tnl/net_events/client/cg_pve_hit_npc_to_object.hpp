#ifndef CG_PVE_HIT_NPC_TO_OBJECT_H
#define CG_PVE_HIT_NPC_TO_OBJECT_H

#include "GameNetEvent.h"

class CGPvEHitNpcToObject final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U64 unk_01; // 88 92
	U32 npcUid; // 96 this normally is objectUid, however, with our patch, this is npcUid. We can just assume object uid is 1

	CGPvEHitNpcToObject() : GameNetEvent{ CG_PVE_HIT_NPC_TO_OBJECT, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&unk_01);
		bstream->read(&npcUid);
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

		const auto& npc = roomSession->getNpcManager()->findNpcByUid(npcUid);

		if (npc == nullptr)
		{
			return;
		}

		// Note: We have hardcoded the uid for the essence object to be 1 so we only check for that one.
		const auto essenceObject = roomSession->getObjectManager()->findObjectByUid(1);

		if (essenceObject == nullptr || essenceObject->getType() != eObjectType::ESSENCE)
		{
			return;
		}

		if (roomSession->getRoom()->getMasterId() == player->getId())
		{
			essenceObject->onHitByNpc(roomSession, npc);
		}
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitNpcToObject>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitNpcToObject);
};
#endif
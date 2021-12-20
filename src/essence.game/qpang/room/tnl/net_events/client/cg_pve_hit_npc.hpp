#ifndef CG_PVE_HIT_NPC_H
#define CG_PVE_HIT_NPC_H

#include "GameNetEvent.h"

class CGPvEHitNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	enum HitLocation : uint8_t
	{
		FRONT = 1,
		BACK_SIDE = 3,
		BACK = 2
	};

	// TODO: Create enum for weaponType

	U32 playerId; // 140
	U32 targetNpcUid; // 144
	U32 unk_03; // 88
	F32 impactPosX; // 116
	F32 impactPosY; // 120
	F32 impactPosZ; // 124
	F32 impactOffsetPosX; // 128
	F32 impactOffsetPosY; // 132
	F32 impactOffsetPosZ; // 136
	U32 unk_10; // 92
	U8 unk_11; // 148 
	U8 bodyPartId; // 149 # differs per npc
	U32 weaponItemId; // 96
	U64 weaponCardId; // 108 104
	U8 weaponType; // 112
	U32 bodyPartIdCopy; // 156
	U8 hitLocation; // 150
	U8 unk_18; // 151
	U32 unk_19; // 152

	CGPvEHitNpc() : GameNetEvent{ CG_PVE_HIT_NPC, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}
	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&playerId);
		bstream->read(&targetNpcUid);
		bstream->read(&unk_03);
		bstream->read(&impactPosX);
		bstream->read(&impactPosY);
		bstream->read(&impactPosZ);
		bstream->read(&impactOffsetPosX);
		bstream->read(&impactOffsetPosY);
		bstream->read(&impactOffsetPosZ);
		bstream->read(&unk_10);
		bstream->read(&unk_11);
		bstream->read(&bodyPartId);
		bstream->read(&weaponItemId);
		bstream->read(&weaponCardId);
		bstream->read(&weaponType);
		bstream->read(&bodyPartIdCopy);
		bstream->read(&hitLocation);
		bstream->read(&unk_18);
		bstream->read(&unk_19);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		std::cout << std::endl;
		std::cout << "============ CGPvEHitNpc Report ============" << std::endl;
		std::cout << "playerId: " << (int)playerId << std::endl;
		std::cout << "targetNpcUid: " << (int)targetNpcUid << std::endl;
		std::cout << std::endl;
		std::cout << "unk_03: " << (int)unk_03 << std::endl;
		std::cout << std::endl;
		std::cout << "impactPosX: " << (float)impactPosX << std::endl;
		std::cout << "impactPosY: " << (float)impactPosY << std::endl;
		std::cout << "impactPosZ: " << (float)impactPosZ << std::endl;
		std::cout << std::endl;
		std::cout << "impactOffsetPosX: " << (float)impactOffsetPosX << std::endl;
		std::cout << "impactOffsetPosY: " << (float)impactOffsetPosY << std::endl;
		std::cout << "impactOffsetPosZ: " << (float)impactOffsetPosZ << std::endl;
		std::cout << std::endl;
		std::cout << "unk_10: " << (int)unk_10 << std::endl;
		std::cout << "unk_11: " << (int)unk_11 << std::endl;
		std::cout << std::endl;
		std::cout << "bodyPartId: " << (int)bodyPartId << std::endl;
		std::cout << std::endl;

		std::cout << "weaponItemId: " << (int)weaponItemId << std::endl;
		std::cout << "weaponCardId: " << (int)weaponCardId << std::endl;
		std::cout << "weaponType: " << (int)weaponType << std::endl;

		std::cout << std::endl;

		std::cout << "bodyPartIdCopy: " << (int)bodyPartIdCopy << std::endl;
		std::cout << std::endl;
		std::cout << "hitLocation: " << (int)hitLocation << std::endl;
		std::cout << std::endl;
		std::cout << "unk_18: " << (int)unk_18 << std::endl;
		std::cout << "unk_19: " << (int)unk_19 << std::endl;
		std::cout << "============================================" << std::endl;
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEHitNpc>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEHitNpc);
};
#endif
#ifndef GC_MASTER_LOG_HPP
#define GC_MASTER_LOG_HPP

#include "GameNetEvent.h"

class GCMasterLog : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCMasterLog() : GameNetEvent{ GC_MASTERLOG, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	// This one is for npc shoot
	GCMasterLog(U32 npcUid, U32 npcWeaponBodyPartId, Position shootTargetPosition, U64 a4) : GameNetEvent{ GC_MASTERLOG, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		unk_01 = npcUid;
		unk_02 = npcWeaponBodyPartId;
		unk_07 = *(U32*)&shootTargetPosition.x;
		unk_08 = *(U32*)&shootTargetPosition.y;
		unk_09 = *(U32*)&shootTargetPosition.z;
		unk_10 = (U32)((a4 & 0xFFFFFFFF00000000LL) >> 32);
		unk_11 = (U32)(a4 & 0xFFFFFFFFLL);
	};

	// This one is for Pve result
	GCMasterLog(uint32_t playerId, bool win, uint32_t goldCoinsEarned, uint32_t silverCoinsEarned, uint32_t bronzeCoinsEarned, uint32_t bestTimeLeftInMs,
		uint32_t currentTimeLeftInMs) : GameNetEvent{ GC_MASTERLOG, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		unk_01 = playerId; // player id
		unk_02 = goldCoinsEarned; // gold coins earned
		unk_03 = silverCoinsEarned; // silver coins earned
		unk_07 = bronzeCoinsEarned; // bronze coins earned
		unk_06 = win; // 0 = lose, 1 = win
		unk_08 = bestTimeLeftInMs; // best time left in ms
		unk_09 = currentTimeLeftInMs; // current time left in ms
		unk_12 = 1337; // Needs to be 1337, it's an identifier for the tool
	};

	// This one is for special attack
	GCMasterLog(uint32_t npcUid, const uint32_t unk02, const uint8_t resetAttack) : GameNetEvent{ GC_MASTERLOG, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		unk_01 = npcUid; // npc uid
		unk_02 = unk02; // unknown variable
		unk_06 = resetAttack; // reset attack (1: npc goes to previous pos, 0: npc goes to y 20)
		unk_12 = 1338; // Needs to be 1338, it's an identifier for the tool
	};

	U32 unk_01; // 88
	U32 unk_02; // 92
	U32 unk_03; // 96
	U16 unk_04; // 100
	U16 unk_05; // 102
	U8 unk_06; // 104
	U32 unk_07; // 108
	U32 unk_08; // 112
	U32 unk_09; // 116
	U32 unk_10; // 120
	U32 unk_11; // 124
	U32 unk_12; // 128
	U8 unk_13; // 132
	ByteBuffer buffer = ByteBuffer(0);

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(unk_05);
		bstream->write(unk_06);
		bstream->write(unk_07);
		bstream->write(unk_08);
		bstream->write(unk_09);
		bstream->write(unk_10);
		bstream->write(unk_11);
		bstream->write(unk_12);
		bstream->write(unk_13);
		bstream->write(&buffer);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};
	TNL_DECLARE_CLASS(GCMasterLog);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
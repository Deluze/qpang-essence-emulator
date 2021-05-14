#ifndef GC_HIT_HPP
#define GC_HIT_HPP

#include "GameNetEvent.h"

class GCHit : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	GCHit() : GameNetEvent{ GC_HIT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	GCHit(
		U32 srcId, U32 dstId, U32 unk_03, F32 srcPosX, F32 srcPosY, F32 srcPosZ, F32 dstPosX, F32 dstPosY, F32 dstPosZ, U32 count, U8 hitType,
		U8 hitLocation, U16 healthLeft, U16 damage, U32 weaponId, U64 rtt, U32 weaponType, U8 unk_16, U8 killCombo, U8 unk_18, U8 effect,
		U8 unk_20 = 0, U8 unk_21 = 0, U32 unk_22 = 0) :
		GameNetEvent{ GC_HIT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient },
		srcPlayerId(srcId),
		dstPlayerId(dstId),
		unk_03(unk_03),
		srcPosX(srcPosX),
		srcPosY(srcPosY),
		srcPosZ(srcPosZ),
		dstPosX(dstPosX),
		dstPosY(dstPosY),
		dstPosZ(dstPosZ),
		count(count),
		hitType(hitType),
		hitLocation(hitLocation),
		damageDealt(damage),
		healthLeft(healthLeft),
		weaponId(weaponId),
		rtt(rtt),
		weaponType(weaponType),
		unk_16(unk_16),
		killCombo(killCombo),
		unk_18(unk_18),
		weaponEffect(effect),
		unk_20(unk_20),
		unk_21(unk_21),
		unk_22(unk_22)
	{}

	GCHit(U32 ownerPlayerId, U32 playerId, U16 damageDealt, U16 healthLeft) : GameNetEvent{ GC_HIT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->srcPlayerId = ownerPlayerId;
		this->dstPlayerId = playerId;
		this->damageDealt = damageDealt;
		this->healthLeft = healthLeft;
	};

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(srcPlayerId);
		bstream->write(dstPlayerId);
		bstream->write(unk_03);
		bstream->write(srcPosX);
		bstream->write(srcPosY);
		bstream->write(srcPosZ);
		bstream->write(dstPosX);
		bstream->write(dstPosY);
		bstream->write(dstPosZ);
		bstream->write(count);
		bstream->write(hitType);
		bstream->write(hitLocation);
		bstream->write(healthLeft);
		bstream->write(damageDealt);
		bstream->write(weaponId);
		bstream->write(rtt);
		bstream->write(weaponType);
		bstream->write(unk_16);
		bstream->write(killCombo);
		bstream->write(unk_18);
		bstream->write(weaponEffect);
		bstream->write(unk_20);
		bstream->write(unk_21);
		bstream->write(unk_22);
		bstream->writeFlag(false); // is p2p
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 srcPlayerId;
	U32 dstPlayerId;
	U32 unk_03;
	F32 srcPosX;
	F32 srcPosY;
	F32 srcPosZ;
	F32 dstPosX;
	F32 dstPosY;
	F32 dstPosZ;
	U32 count; //??
	U8 hitType;
	U8 hitLocation;
	U16 healthLeft;
	U16 damageDealt;
	U32 weaponId;
	U64 rtt;
	U32 weaponType;
	U8 unk_16;
	U8 killCombo;
	U8 unk_18;
	U8 weaponEffect; // debuff
	U8 unk_20;
	U8 unk_21;
	U32 unk_22;

	TNL_DECLARE_CLASS(GCHit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
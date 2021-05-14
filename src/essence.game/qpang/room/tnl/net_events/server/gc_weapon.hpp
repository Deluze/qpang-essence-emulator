#ifndef GC_WEAPON_HPP
#define GC_WEAPON_HPP

#include "GameNetEvent.h"

class GCWeapon : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	GCWeapon() : GameNetEvent{ GC_WEAPON, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	GCWeapon(U32 playerId, U32 cmd, U32 weaponId, U64 weaponSeqId, U16 weaponOpt = 0, U8 outOfBullet = false, U16 magazineId = 75) :
		GameNetEvent{ GC_WEAPON, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient },
		playerId(playerId),
		cmd(cmd),
		weaponId(weaponId),
		weaponSeqId(weaponSeqId),
		weaponOpt(weaponOpt),
		outOfBullet(outOfBullet),
		magazineId(magazineId)
	{
	};

	GCWeapon(U32 playerId, U32 cmd, U16 weaponOpt = 0) : GameNetEvent{ GC_WEAPON, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->playerId = playerId;
		this->cmd = cmd;
		this->weaponOpt = weaponOpt;
	};

	U32 playerId = 0;
	U32 cmd = 100;	
	U32 weaponId = 0;
	U64 weaponSeqId = 0;
	U16 weaponOpt = 0; // certain types.. poison, freeze, stun etc.
	U8 outOfBullet = false;
	U16 magazineId = 75;

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(playerId);
		bstream->write(cmd); // 0 = switch, 1 = gun at ground zero??, 3 = reload
		bstream->write(weaponId);
		bstream->write(weaponSeqId);
		bstream->write(weaponOpt);
		bstream->write(outOfBullet);
		bstream->write(magazineId); // 75 or 76??
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCWeapon);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
#ifndef GC_PVE_SPECIAL_ATTACK_HPP
#define GC_PVE_SPECIAL_ATTACK_HPP


#include "GameNetEvent.h"

class GCPvESpecialAttack : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvESpecialAttack() : GameNetEvent{ GC_PVE_SPECIAL_ATTACK, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvESpecialAttack);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
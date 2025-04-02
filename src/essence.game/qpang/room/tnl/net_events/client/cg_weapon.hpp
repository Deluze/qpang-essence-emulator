#ifndef CG_WEAPON_H
#define CG_WEAPON_H

#include "GameNetEvent.h"

#include "qpang/room/tnl/net_events/server/gc_weapon.hpp"

class GCWeapon;

class CGWeapon : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum CMD : U32
	{
		SWAP = 0,
		EQUIP_MACHINE_GUN = 1,
		UNEQUIP_MACHINE_GUN = 2,
		RELOAD = 3,
		ENABLE_SHOOTING = 5,
	};

	CGWeapon() : GameNetEvent{ CG_WEAPON, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
		bstream->read(&itemId);
		bstream->read(&seqId);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (auto session = roomPlayer->getRoomSessionPlayer(); session != nullptr)
			{
				if (cmd == CMD::SWAP)
				{
					if (!session->getWeaponManager()->hasWeapon(itemId))
						return;

					auto isReloadGlitchEnabled = session->getRoomSession()->getRoom()->isReloadGlitchEnabled();

					session->getWeaponManager()->switchWeapon(itemId, isReloadGlitchEnabled);
				}
				else if (cmd == CMD::RELOAD)
				{
					if (!session->getWeaponManager()->hasWeapon(itemId))
						return;

					if (!session->getWeaponManager()->canReload())
						return;

					session->getWeaponManager()->reload(seqId);
				}
				else if (cmd == CMD::UNEQUIP_MACHINE_GUN)
				{
					// Check for the ground zero map.
					if (roomPlayer->getRoom()->getMap() == 8)
					{
						session->getWeaponManager()->unequipMachineGun();
					}
				}
				else if (cmd == CMD::EQUIP_MACHINE_GUN)
				{
					// Check for the ground zero map.
					if (roomPlayer->getRoom()->getMap() == 8)
					{
						session->getWeaponManager()->equipMachineGun(seqId);
					}
				}
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGWeapon>(ps);
	};

	U32 playerId;
	U32 cmd;
	U32 itemId;
	U64 seqId;

	TNL_DECLARE_CLASS(CGWeapon);
};
#endif
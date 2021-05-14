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
		bstream->read(&cardId); // can be omitted
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

					session->getWeaponManager()->switchWeapon(itemId);
				}
				else if (cmd == CMD::RELOAD)
				{
					if (!session->getWeaponManager()->hasWeapon(itemId))
						return;

					if (!session->getWeaponManager()->canReload())
						return;

					session->getWeaponManager()->reload(cardId);
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
	U64 cardId;

	TNL_DECLARE_CLASS(CGWeapon);
};
#endif
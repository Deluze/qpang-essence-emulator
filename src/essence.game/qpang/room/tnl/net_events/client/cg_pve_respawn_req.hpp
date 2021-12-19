#ifndef CG_PVE_RESPAWN_REQ_H
#define CG_PVE_RESPAWN_REQ_H

#include "GameNetEvent.h"
class CGPvERespawnReq : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvERespawnReq() : GameNetEvent{ CG_PVE_RESPAWN_REQ, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		// TODO: Send packet back, asking for respawn.
		/*int userId = player->getUserId();
		player->getRoomPlayer()->getRoomSessionPlayer()->getRoomSession()->relayPlaying<GCGameState>(userId, 16);*/
	}

	void unpack(EventConnection* conn, BitStream* bstream) 
	{

	};

	void process(EventConnection* ps) 
	{
		post<CGPvERespawnReq>(ps);
	};

	TNL_DECLARE_CLASS(CGPvERespawnReq);
};
#endif
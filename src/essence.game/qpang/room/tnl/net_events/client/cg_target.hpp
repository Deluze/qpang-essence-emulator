#ifndef CG_TARGET_H
#define CG_TARGET_H

#include "GameNetEvent.h"

class CGTarget : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGTarget() : GameNetEvent{ CG_TARGET, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&cmd);
		bstream->read(&playerId);
		bstream->read(&targetId);
	};
	void process(EventConnection* ps)
	{
		//auto player = sGame->FindPlayer(ps, playerId);

		//if (player)
		//{
		//	auto room = player->GetRoom();
		//
		//	if (room && room->GetPlayer(targetId))
		//	{
		//		room->OnPlayerTarget(*this);
		//	}
		//}
	};

	U32 playerId;
	U32 cmd;
	U32 targetId;

	TNL_DECLARE_CLASS(CGTarget);
};
#endif
#ifndef CG_MOTION_H
#define CG_MOTION_H

#include "GameNetEvent.h"

class GCMotion;

class CGMotion : public GameNetEvent
{

	typedef NetEvent Parent;
public:
	CGMotion() : GameNetEvent{ CG_MOTION, NetEvent::GuaranteeType::Unguaranteed, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&cmd);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
		bstream->read(&unk_07);
		bstream->read(&unk_08);
		bstream->read(&unk_09);
		bstream->read(&playerId);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
			if (auto roomSession = roomPlayer->getRoom()->getRoomSession(); roomSession != nullptr)
				roomSession->relayPlaying<GCMotion>(cmd, unk_02, unk_03, unk_04, unk_05, unk_06, unk_07, unk_08, unk_09, playerId);
	}

	void process(EventConnection* ps)
	{
		post<CGMotion>(ps);
	};

	U32 cmd;
	U32 unk_02;
	U32 unk_03;
	U32 unk_04;
	U32 unk_05;
	U32 unk_06;
	U32 unk_07;
	U32 unk_08;
	U32 unk_09;
	U32 playerId;

	TNL_DECLARE_CLASS(CGMotion);
};
#endif
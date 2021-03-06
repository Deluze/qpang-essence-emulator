#ifndef CG_MOVE_H
#define CG_MOVE_H

#include "GameNetEvent.h"

class GCMove;

class CGMove : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGMove() : GameNetEvent{ CG_MOVE, NetEvent::GuaranteeType::Unguaranteed, NetEvent::DirClientToServer } {};
	U32 playerId = 0;
	U32 cmd = 0;
	F32 unk_01 = 0;
	F32 unk_02 = 0;
	F32 unk_03 = 0;
	F32 unk_04 = 0;
	F32 unk_05 = 0;
	F32 unk_06 = 0;
	F32 unk_07 = 0;
	F32 unk_08 = 0;
	F32 unk_09 = 0;
	F32 unk_10 = 0;
	U32 unk_11 = 1;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
		bstream->read(&unk_07);
		bstream->read(&unk_08);
		bstream->read(&unk_09);
		bstream->read(&unk_10);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (roomPlayer->isSpectating())
				return;

			if (auto session = roomPlayer->getRoomSessionPlayer(); session != nullptr)
			{
				if (session->isDead()) // if they want to try and revive themselves
					return;

				session->setPosition({ unk_01, unk_02, unk_03 });
			}

			if (auto roomSession = roomPlayer->getRoom()->getRoomSession(); roomSession != nullptr)
				roomSession->relayPlayingExcept<GCMove>(player->getId(), playerId, cmd, unk_01, unk_02, unk_03, unk_04, unk_05, unk_06, unk_07, unk_08, unk_09, unk_10);
		}
	}

	void process(EventConnection* ps)
	{
		post<CGMove>(ps);
	};

	TNL_DECLARE_CLASS(CGMove);
};
#endif
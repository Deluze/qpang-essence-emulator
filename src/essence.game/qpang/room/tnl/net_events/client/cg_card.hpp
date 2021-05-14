#ifndef CG_CARD_HPP
#define CG_CARD_HPP

#include "GameNetEvent.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/RoomPlayer.h"

class GCCard;

class CGCard : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum CMD : U32
	{
		ABILITY = 0x07,
		USE_CARD = 0x09,
	};

	CGCard() : GameNetEvent{ CG_CARD, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&uid);
		bstream->read(&targetUid);
		bstream->read(&cmd);
		bstream->read(&cardType);
		bstream->read(&itemId);
		bstream->read(&seqId);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (auto roomSession = roomPlayer->getRoom()->getRoomSession(); roomSession != nullptr)
			{
				roomSession->relayPlaying<GCCard>(uid, targetUid, cmd, cardType, itemId, seqId);
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGCard>(ps);
	};

	U32 uid;
	U32 targetUid;
	U32 cmd;
	U32 cardType;
	U32 itemId;
	U64 seqId;

	TNL_DECLARE_CLASS(CGCard);
};
#endif
#ifndef GC_CARD_HPP
#define GC_CARD_HPP

#include "GameNetEvent.h"
#include "cg_card.hpp"

class GCCard : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCCard() : GameNetEvent{ GC_CARD, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {}
	;


	GCCard(U32 playerId, U32 targetId, U8 cmd, U32 cardType, U32 itemId, U64 seqId) : GameNetEvent{ GC_CARD, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->uid = playerId;
		this->targetUid = targetId;
		this->cmd = cmd;
		this->cardType = cardType;
		this->itemId = itemId;
		this->seqId = seqId;
	};

	GCCard(U32 playerId, U32 guagePercentage, U32 guagePoints)
	{
		this->uid = playerId;
		this->cmd = 8;
		this->cardType = 9; // NEEDS TO BE 9 SKILL RELATED 
		this->chargePoint = guagePercentage;
		this->skillCount = guagePoints;
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(cmd);
		bstream->write(uid);
		bstream->write(targetUid);
		bstream->write(itemId);
		bstream->write(seqId);
		bstream->write(cardType);
		bstream->write(actionType);
		bstream->write(chargePoint);
		bstream->write(skillCount);
		bstream->write(leftCount);
		bstream->write(dataSrcUid);
		bstream->write(dataTrgUid);
		bstream->write(unk_01);
		bstream->write(count);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U8 cmd = 0;
	U32 uid = 0;
	U32 targetUid = 0;
	U32 itemId = 0;
	U64 seqId = 0;
	U32 cardType = 0;
	U32 actionType = 0;
	U32 chargePoint = 0;
	U32 skillCount = 0;
	U32 leftCount = 0;
	U32 dataSrcUid = 0;
	U32 dataTrgUid = 0;
	U32 unk_01 = 0;
	U8 count = 0;

	TNL_DECLARE_CLASS(GCCard);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
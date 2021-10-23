#ifndef GC_CARD_HPP
#define GC_CARD_HPP

#include "GameNetEvent.h"

class GCCard : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCCard() : GameNetEvent{ GC_CARD, GuaranteedOrdered, DirAny } {};

	GCCard(const U32 playerId, const U32 targetId, const U8 cmd, const U32 cardType, const U32 itemId, const U64 seqId,
		const std::vector<uint32_t>& playerIds = {})
		: GameNetEvent{ GC_CARD, Guaranteed, DirServerToClient }
	{
		this->uid = playerId;
		this->targetUid = targetId;
		this->dataSrcUid = playerId;
		this->dataTrgUid = targetId;
		this->cmd = cmd;
		this->cardType = cardType;
		this->itemId = itemId;
		this->seqId = seqId;

		this->playerCount = static_cast<U8>(playerIds.size());
		this->playerIds = playerIds;
	}

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
		bstream->write(playerCount);

		for (const auto playerId : playerIds)
		{
			bstream->write(playerId);
		}
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U8 cmd = 0;
	U32 uid = 0;
	U32 targetUid = 0;
	U32 itemId = 0;
	U64 seqId = 0;
	U32 cardType = 0;
	U32 actionType = 1; // 1 = active, 0 = not active?
	U32 chargePoint = 0;
	U32 skillCount = 0; // The amount of points it costs to use the card.
	U32 leftCount = 0; // The amount of uses this card has?
	U32 dataSrcUid = 0;
	U32 dataTrgUid = 0;
	U32 unk_01 = 0;

	U8 playerCount = 0; // Amount of team or enemy players.
	std::vector<uint32_t> playerIds; // Vector full of team/enemy player ids.

	TNL_DECLARE_CLASS(GCCard);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
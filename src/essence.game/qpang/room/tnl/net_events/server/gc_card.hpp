#ifndef GC_CARD_HPP
#define GC_CARD_HPP

#include "GameNetEvent.h"

class GCCard : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCCard() : GameNetEvent{ GC_CARD, GuaranteedOrdered, DirAny } {};

	GCCard(const U32 playerId, const U32 targetId, const U8 cmd, const U32 cardType, const U32 itemId, const U64 seqId,
		const std::vector<uint32_t>& affectedPlayerIds = {})
		: GameNetEvent{ GC_CARD, Guaranteed, DirServerToClient }
	{
		this->uid = playerId;
		this->targetUid = targetId;
		this->dataSrcUid = playerId;
		this->dataTargetUid = targetId;
		this->cmd = cmd;
		this->cardType = cardType;
		this->itemId = itemId;
		this->seqId = seqId;

		this->affectedPlayersCount = static_cast<U8>(affectedPlayerIds.size());
		this->affectedPlayerIds = affectedPlayerIds;
	}

	GCCard(const U32 playerId, const U32 targetId, const U8 cmd, const U32 cardType, const U32 itemId, const U64 seqId,
		const U32 usesLeftCount, const std::vector<uint32_t>& affectedPlayerIds = {})
		: GameNetEvent{ GC_CARD, Guaranteed, DirServerToClient }
	{
		this->uid = playerId;
		this->targetUid = targetId;
		this->dataSrcUid = playerId;
		this->dataTargetUid = targetId;
		this->cmd = cmd;
		this->cardType = cardType;
		this->itemId = itemId;
		this->seqId = seqId;

		this->usesLeftCount = usesLeftCount;

		this->affectedPlayersCount = static_cast<U8>(affectedPlayerIds.size());
		this->affectedPlayerIds = affectedPlayerIds;
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
		bstream->write(usesLeftCount);
		bstream->write(dataSrcUid);
		bstream->write(dataTargetUid);
		bstream->write(unk_01);
		bstream->write(affectedPlayersCount);

		for (const auto playerId : affectedPlayerIds)
		{
			bstream->write(playerId);
		}
	}

	void unpack(EventConnection* conn, BitStream* bstream) {}
	void process(EventConnection* ps) {}

	U8 cmd = 0;
	U32 uid = 0;
	U32 targetUid = 0;
	U32 itemId = 0;
	U64 seqId = 0;
	U32 cardType = 0;
	// Don't think it matters, but it is set to 1 in the configs (1 = active, 0 = not active).
	U32 actionType = 1;
	U32 chargePoint = 0;
	U32 skillCount = 0;
	// The amount of times a card can be used.
	U32 usesLeftCount = 0;
	// Set equal to uid.
	U32 dataSrcUid = 0;
	// Set equal to targetuid.
	U32 dataTargetUid = 0;
	// I think it disables a certain animation (see reflect skill) with cmd 3.
	U32 unk_01 = 1;

	// Amount of players that are affected by the skill.
	U8 affectedPlayersCount = 0;
	// Set of players that are affected by the skill.
	std::vector<uint32_t> affectedPlayerIds{};

	TNL_DECLARE_CLASS(GCCard);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
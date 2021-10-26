#ifndef GC_GAME_STATE_HPP
#define GC_GAME_STATE_HPP

#include <vector>

#include "GameNetEvent.h"

class GCGameState : public GameNetEvent
{
	typedef NetEvent Parent;

public:
	enum BonusId : uint16_t
	{
		BONUS_YOU_GOT_NO_DEATH = 2,
		BONUS_MULTI_KILL = 5,
		BONUS_COMBO = 6,
		BONUS_MVP = 8,
		BONUS_FRIENDS = 11,
		BONUS_EXTRA_EXP_EVENT = 12,
		BONUS_EXTRA_DON_EVENT = 13, // killing pros?
		BONUS_KILLING_PROS = 14, // first kill
		BONUS_FIRST_KILL = 15, // first mission
		BONUS_FIRST_MATCH_GAME = 16, // start off
		BONUS_FIRST_MISSION = 17,
		BONUS_START_OFF_EXP = 18,
		BONUS_Q_BONUS = 19,
		BONUS_EXP_MAKER = 20,
		BONUS_MONEY_MAKER = 21,
		BONUS_PC_CAFE_DON = 22,
		BONUS_PC_CAFE_EXP = 23,
		BONUS_CANDY_AMOUNT = 24,
		BONUS_COIN_BOOSTER = 25,
	};

	struct BonusInfo
	{
		U16 mId;
		F32 mExp;
		F32 mDon;
		U8 mOp;
		U8 mType;
	};

	GCGameState() : GameNetEvent{ GC_GAME_STATE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {}

	GCGameState(U32 playerId, U8 cmd, const U32 value = 0, const U32 dataUid = 0) : GameNetEvent{ GC_GAME_STATE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->uid = playerId;
		this->cmd = cmd;
		this->data = value;
		this->dataUid = dataUid;
	}

	GCGameState(const RoomSessionPlayer::Ptr& player, U8 cmd, const U32 value = 0, const U32 dataUid = 0)
		: GameNetEvent{ GC_GAME_STATE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->uid = player->getPlayer()->getId();
		this->cmd = cmd;
		this->data = value;
		this->dataUid = dataUid;

		this->gainExp = player->getExperience();
		this->gainDon = player->getDon();

		this->unk_01 = player->getPlayer()->getAchievementContainer()->listRecent();
	}

	GCGameState(const RoomSessionPlayer::Ptr& player, const U8 cmd, const std::vector<BonusInfo>& bonusInfoList)
		: GameNetEvent{ GC_GAME_STATE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->uid = player->getPlayer()->getId();
		this->cmd = cmd;
		this->mBonusInfoList = bonusInfoList;
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(uid);
		bstream->write(cmd);
		bstream->write(data);
		bstream->write(dataUid);
		bstream->write(gainExp);
		bstream->write(gainDon);
		bstream->write(U8(unk_01.size()));

		for (int i = 0; i < unk_01.size(); i++)
		{
			bstream->write(U16(unk_01[i]));
		}

		bstream->write(static_cast<U8>(mBonusInfoList.size()));

		for (const auto& [id, exp, don, op, type] : mBonusInfoList)
		{
			bstream->write(id);
			bstream->write(exp);
			bstream->write(don);
			bstream->write(op);
			bstream->write(type);
		}
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 uid = 0;
	U8 cmd = 0;
	U32 data = 0;
	U32 dataUid = 0;
	U16 gainExp = 0;
	U16 gainDon = 0;

	U8 counter = 0;
	std::vector<uint32_t> unk_01 = {}; //size: mCounter

	std::vector<BonusInfo> mBonusInfoList = {};

	TNL_DECLARE_CLASS(GCGameState);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
#ifndef GC_GAME_STATE_HPP
#define GC_GAME_STATE_HPP

#include <vector>

#include "GameNetEvent.h"

class GCGameState final : public GameNetEvent
{
	typedef NetEvent Parent;

public:
	enum class ePvpBonusId : U16
	{
		NO_DEATHS = 2,
		MULTI_KILL = 5,
		COMBO_KILL = 6,
		MVP = 8,
		FRIEND = 11,
		EVENT_EXP_AND_DON = 12, // Can be bonus-18 (extra exp event) or bonus-19 (extra don event) based on given exp and/or don.
		KILLING_PROS = 13,
		FIRST_KILL = 14,
		FIRST_MATCH_GAME = 15,
		FIRST_MISSION = 16,
		START_OFF_EXP = 17,
		Q_BONUS = 18,
		EXP_MAKER = 19,
		MONEY_MAKER = 20,
		PC_CAFE_EXP_AND_DON = 21, // Can be bonus-21 (pc cafe exp) or bonus-20 (pc cafe don) based on given exp and/or don.
		ITEM_DROP_EVENT = 22, // Based on the current event item drop (wallie, qcoin etcetera..)
		COIN_BOOSTER = 23
	};

	enum class ePveBonusId : U16
	{
		COMBO_KILL = 6,
		OMPHALOS_BOX_01 = 24,
		PANTHALASSA_BOX_01 = 25,
	};

	struct BonusInfo
	{
		U16 uid;
		F32 gainedExp;
		F32 gainedDon;
		U8 op;
		U8 type;
	};

	U32 uid = 0;
	U8 cmd = 0;
	U32 data = 0;
	U32 dataUid = 0;
	U16 gainedExp = 0;
	U16 gainedDon = 0;

	U8 counter = 0;

	std::vector<uint32_t> achievementIds = {};
	std::vector<BonusInfo> bonusInfo = {};

	GCGameState() : GameNetEvent{ GC_GAME_STATE, Guaranteed, DirServerToClient } {}

	GCGameState(const U32 playerId, const U8 cmd, const U32 value = 0, const U32 dataUid = 0)
		: GameNetEvent{ GC_GAME_STATE, Guaranteed, DirServerToClient },
		uid(playerId),
		cmd(cmd),
		data(value),
		dataUid(dataUid)
	{
	}

	GCGameState(const RoomSessionPlayer::Ptr& player, const U8 cmd, const U32 value = 0, const U32 dataUid = 0)
		: GameNetEvent{ GC_GAME_STATE, Guaranteed, DirServerToClient },
		cmd(cmd),
		data(value),
		dataUid(dataUid),
		gainedExp(player->getExperience()),
		gainedDon(player->getDon())
	{
		this->uid = player->getPlayer()->getId();
		this->achievementIds = player->getPlayer()->getAchievementContainer()->listRecent();
	}

	GCGameState(const RoomSessionPlayer::Ptr& player, const U8 cmd, std::vector<BonusInfo> bonusInfo)
		: GameNetEvent{ GC_GAME_STATE, Guaranteed, DirServerToClient },
		cmd(cmd),
		bonusInfo(std::move(bonusInfo))
	{
		this->uid = player->getPlayer()->getId();
	}

	void pack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->write(uid);
		bstream->write(cmd);
		bstream->write(data);
		bstream->write(dataUid);
		bstream->write(gainedExp);
		bstream->write(gainedDon);

		bstream->write(static_cast<U8>(achievementIds.size()));

		for (size_t i = 0; i < achievementIds.size(); i++)
		{
			bstream->write(static_cast<U16>(achievementIds[i]));
		}

		bstream->write(static_cast<U8>(bonusInfo.size()));

		for (const auto& [bonusUid, exp, don, op, type] : bonusInfo)
		{
			bstream->write(bonusUid);
			bstream->write(exp);
			bstream->write(don);
			bstream->write(op);
			bstream->write(type);
		}
	};

	void unpack(EventConnection* conn, BitStream* bstream) override {}
	void process(EventConnection* ps) override {}

	TNL_DECLARE_CLASS(GCGameState);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
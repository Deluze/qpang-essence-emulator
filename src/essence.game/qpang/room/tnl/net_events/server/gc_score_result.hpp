#ifndef GC_SCORE_RESULT_HPP
#define GC_SCORE_RESULT_HPP

#include "GameNetEvent.h"

#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

class GCScoreResult : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	GCScoreResult() : GameNetEvent{ GC_SCORE_RESULT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCScoreResult(const RoomSession::Ptr roomSession, const std::vector<RoomSessionPlayer::Ptr>& players) : GameNetEvent{ GC_SCORE_RESULT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->players = players;

		if (roomSession->getGameMode()->isTeamMode())
		{
			this->blueScore = this->blueKills = roomSession->getBluePoints();
			this->yellowScore = this->yellowKills = roomSession->getYellowPoints();
		}
		else
			this->blueScore = roomSession->getTopScore();
	};

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(U16(blueScore)); // total essence blue
		bstream->write(U16(blueKills)); // total kills blue
		bstream->write(U16(blueDeaths)); // total deaths blue
		bstream->write(U16(yellowScore)); // total essence yellow
		bstream->write(U16(yellowKills)); // total kills yellow
		bstream->write(U16(yellowDeaths)); // total deaths yellow
		bstream->write(U16(0));
		bstream->write(U16(0));
		bstream->write(U16(0));
		bstream->write(U16(0));
		bstream->write(U16(0));
		bstream->write(U16(0));

		U8 playerCount = players.size();
		if (players.size() > 16)
			playerCount = 16;
		
		bstream->write(playerCount);
		bstream->write(U32(0));

		for (const auto& player : players)
		{
			U8 playerIndex = 0;

			auto roomSession = player->getRoomSession();
			auto isPublicEnemyMode = roomSession->getGameMode()->isPublicEnemyMode();
			
			bstream->write(U32(player->getPlayer()->getId()));
			bstream->write(U16(0)); // unk_02
			bstream->write(U8(0)); // unk_03
			bstream->write(U8(player->getTeam()));
			bstream->write((U16)player->getScore()); /// essence
			// Tag kills as player or player kills.
			bstream->write((U16)(isPublicEnemyMode)
				? player->getTagKillsAsPlayer()
				: player->getKills());
			bstream->write((U16)player->getDeaths());

			// Player kills as tag or player score.
			bstream->write((U16)(isPublicEnemyMode)
				? player->getPlayerKillsAsTag()
				: player->getScore());
			bstream->write(U32(player->getDon())); // don earned
			bstream->write(U32(player->getExperience())); // xp earned
			bstream->write(U32(0)); // shows weird (B) icon
			bstream->write(U8(!player->getPlayer()->getAchievementContainer()->listRecent().empty())); // achievement earned bool
			bstream->write(U8(player->getEventItemPickUps())); // event item pickup count
			bstream->write(U8(0)); // unk_14
			bstream->write(U8(0)); // unk_15

			bool hasXPBonus = player->getExpRate() > 0;
			bool hasDonBonus = player->getDonRate() > 0;

			bstream->write(U8(hasDonBonus && hasXPBonus ? 3 : hasDonBonus ? 2 : hasXPBonus ? 1 : 0)); // unk_16 1 = exp only, 2 = don only, 3 = both exp rate + don rate
			bstream->write(U16(player->getDonRate()));
			bstream->write(U16(player->getExpRate()));
			writeByteBuffer(bstream, player->getPlayer()->getName(), 16);
			bstream->write(U32((isPublicEnemyMode) ? player->getTagPoints() : 0)); // Tag points

			playerIndex++;
			if (playerIndex >= 16)
				break;
		}

	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	std::vector<RoomSessionPlayer::Ptr> players;

	U16 blueScore = 0;
	U16 blueKills = 0;
	U16 blueDeaths = 0;
	U16 yellowScore = 0;
	U16 yellowKills = 0;
	U16 yellowDeaths = 0;

	TNL_DECLARE_CLASS(GCScoreResult);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
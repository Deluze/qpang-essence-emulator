#ifndef GC_SCORE_HPP
#define GC_SCORE_HPP

#include <numeric>
#include <algorithm>

#include "GameNetEvent.h"

#include "qpang/room/session/RoomSession.h"

class GCScore : public GameNetEvent
{
	typedef NetEvent Parent;

public:

	GCScore() : GameNetEvent{ GC_SCORE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	GCScore(const std::vector<RoomSessionPlayer::Ptr>& players, std::shared_ptr<RoomSession> roomSession, const U8& cmd) : GameNetEvent{ GC_SCORE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->players = players;
		this->cmd = cmd;
		this->unk_04 = roomSession->getElapsedTime();

		if (roomSession->getGameMode()->isTeamMode())
		{
			this->unk_02 = this->blueTotalKill = roomSession->getBluePoints();
			this->unk_03 = this->yellowTotalKill = roomSession->getYellowPoints();
		}
		else
			this->blueTotalKill = roomSession->getTopScore();
	};

	GCScore(std::shared_ptr<RoomSession> roomSession, const U8& cmd) : GameNetEvent{ GC_SCORE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->cmd = cmd;
		this->unk_04 = roomSession->getElapsedTime();

		if (roomSession->getGameMode()->isTeamMode())
		{
			this->unk_02 = this->blueTotalKill = roomSession->getBluePoints();
			this->unk_03 = this->yellowTotalKill = roomSession->getYellowPoints();
		}
		else
			this->blueTotalKill = roomSession->getTopScore();
	};

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(cmd);
		bstream->write(unk_02);
		bstream->write(blueTotalKill);
		bstream->write(blueTotalDeath);
		bstream->write(unk_03);
		bstream->write(yellowTotalKill);
		bstream->write(yellowTotalDeath);
		bstream->write(unk_04);

		U8 playerCount = players.size();
		if (players.size() > 16)
			playerCount = 16;
		
		bstream->write(playerCount);

		uint8_t team0Spot{ 1 };
		uint8_t team1Spot{ 1 };
		uint8_t team2Spot{ 1 };

		U8 playerIndex = 0;
		
		for (const auto& player : players)
		{	
			auto actPlayer = player->getPlayer();

			bstream->write(actPlayer->getId());
			bstream->write((U16)actPlayer->getLevel());
			bstream->write((U8)4);

			bstream->write((U16)player->getScore()); // essence
			bstream->write((U16)player->getKills());
			bstream->write((U16)player->getDeaths());
			bstream->write((U16)player->getScore());
			bstream->write(player->getTeam());

			writeByteBuffer(bstream, actPlayer->getName(), 16);
			bstream->write((U32)player->getPublicEnemyScore()); // Prey kill count
			
			playerIndex++;
			if (playerIndex >= 16)
				break;
		}
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	std::vector<RoomSessionPlayer::Ptr> players;
	U8 cmd = -56;
	U16 unk_02 = 0;
	U16 blueTotalKill = 0;
	U16 blueTotalDeath = 0;
	U16 unk_03 = 0;
	U16 yellowTotalKill = 0;
	U16 yellowTotalDeath = 0;
	U32 unk_04 = 1;

	TNL_DECLARE_CLASS(GCScore);

};
#endif // !GC_ARRANGED_ACCEPT_HPP
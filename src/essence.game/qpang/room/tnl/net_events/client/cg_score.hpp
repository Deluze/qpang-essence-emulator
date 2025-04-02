#ifndef CG_SCORE_H
#define CG_SCORE_H

#include "GameNetEvent.h"
#include "gc_score.hpp"

class CGScore : public GameNetEvent
{
	typedef NetEvent Parent;


public:
	CGScore() : GameNetEvent{ CG_SCORE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};
	U32 cmd;

	enum CMD {
		GAME = 200,
		USER = 100,
	};

	void pack(EventConnection* conn, BitStream* bstream) {};

	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&cmd);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (auto session = roomPlayer->getRoomSessionPlayer(); session != nullptr)
			{
				if (session->getRoomSession()->isFinished())
					return;
				
				if (cmd == CMD::USER)
				{
					auto roomSession = session->getRoomSession();
					
					if (roomSession->isFinished())
						return;
					
					auto players = roomSession->getPlayingPlayers();

					if (roomSession->getGameMode()->isPublicEnemyMode())
					{
						std::sort(players.begin(), players.end(),
							[](RoomSessionPlayer::Ptr& lhs, RoomSessionPlayer::Ptr& rhs)
							{
								return lhs->getTagPoints() > rhs->getTagPoints();
							}
						);
					}
					else
					{
						std::sort(players.begin(), players.end(),
							[](RoomSessionPlayer::Ptr& lhs, RoomSessionPlayer::Ptr& rhs)
							{
								return lhs->getScore() > rhs->getScore();
							}
						);
					}

					session->post(new GCScore(players, roomSession, 1));
				}
				else if (cmd == CMD::GAME)
				{
					session->post(new GCScore(session->getRoomSession(), -56));
				}

				// Send twice to fix spectator position reset
				session->post(new GCGameState(player->getId(), 11, session->getRoomSession()->getElapsedTime()));
				session->post(new GCGameState(player->getId(), 11, session->getRoomSession()->getElapsedTime()));
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGScore>(ps);
	};

	TNL_DECLARE_CLASS(CGScore);
};
#endif
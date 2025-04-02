#ifndef CG_MESG_H
#define CG_MESG_H

#include "GameNetEvent.h"
#include "gc_mesg.hpp"

class CGMesg : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGMesg() : GameNetEvent{ CG_MESG, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer }
	{
		nicknameBuffer = new ByteBuffer();
		messageBuffer = new ByteBuffer();
	};

	enum CMD : uint16_t {
		ALL_CHAT = 0,
		TEAM_CHAT = 8
	};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
		bstream->read(nicknameBuffer);
		bstream->read(messageBuffer);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		std::u16string nickname = byteBufferToString(nicknameBuffer, 16);
		std::u16string message = byteBufferToString(messageBuffer, 254);

		delete nicknameBuffer;
		delete messageBuffer;

		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			auto room = roomPlayer->getRoom();

			auto roomSession = room->getRoomSession();

			message = Game::instance()->getChatManager()->chat(player, message);

			if (player->isMuted() && !message.empty())
				return player->broadcast(u"You are currently restricted from using the chat.");

			if (roomPlayer->isSpectating() && !message.empty()) {
				auto rank = roomPlayer->getPlayer()->getRank();

				if (rank == 1) { // player
					return player->broadcast(u"You may not chat as a spectator.");
				}
			}

			if (message.empty())
				return;

			if (roomSession == nullptr)
			{
				room->broadcast<GCMesg>(player->getId(), cmd, player->getName(), message);
			}
			else
			{
				if (roomPlayer->isPlaying())
				{
					switch (cmd)
					{
					case TEAM_CHAT:
						if (roomSession->getGameMode()->isPublicEnemyMode())
						{
							const auto currentlySelectedTag = roomSession->getCurrentlySelectedTag();
							const auto isPublicEnemy = currentlySelectedTag == player->getId();

							if (isPublicEnemy)
							{
								// Player is public enemy so only send message to other public enemies.
								// In our case, there exists only one public enemy so only send the message to the player itsself.
								roomPlayer->send(new GCMesg(player->getId(), cmd, player->getName(), message));
							}
							else 
							{
								// Player is not a public enemy, so send the message to every player except the public enemy.
								roomSession->relayExcept<GCMesg>(currentlySelectedTag, player->getId(), cmd, player->getName(), message);
							}
						}
						else
						{
							// If we get here this means a team chat message was sent, so lets make sure the gamemode is a team mode.
							if (!roomSession->getGameMode()->isTeamMode())
							{
								return;
							}

							// Now we know it's a team chat message, send the message to only team players.
							const auto teamPlayers = roomSession->getPlayersForTeam(roomPlayer->getTeam());

							for (const auto &teamPlayer : teamPlayers)
							{
								teamPlayer->post(new GCMesg(player->getId(), cmd, player->getName(), message));
							}
						}

						break;
					case ALL_CHAT:
					default:
						roomSession->relay<GCMesg>(player->getId(), cmd, player->getName(), message);
						break;
					}
				}
				else
				{
					room->broadcastWaiting<GCMesg>(player->getId(), cmd, player->getName(), message);
				}
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGMesg>(ps);
	};


	U32 playerId;
	U32 cmd;

	ByteBuffer* nicknameBuffer;
	ByteBuffer* messageBuffer;

	TNL_DECLARE_CLASS(CGMesg);
};
#endif
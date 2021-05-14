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
				return player->broadcast(u"You're currently muted. Your message has not been sent");

			if(roomPlayer->isSpectating() && !message.empty())
				return player->broadcast(u"No! You can't chat as a spectator");

			if (message.empty())
				return;

			if (roomSession == nullptr)
				room->broadcast<GCMesg>(player->getId(), cmd, player->getName(), message);
			else
			{
				if (roomPlayer->isPlaying())
					roomSession->relay<GCMesg>(player->getId(), cmd, player->getName(), message);
				else
					room->broadcastWaiting<GCMesg>(player->getId(), cmd, player->getName(), message);
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
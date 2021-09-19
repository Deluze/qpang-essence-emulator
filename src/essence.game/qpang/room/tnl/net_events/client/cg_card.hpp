#ifndef CG_CARD_HPP
#define CG_CARD_HPP

#include "GameNetEvent.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/RoomPlayer.h"

class GCCard;

class CGCard : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	enum CMD : U32
	{
		ABILITY = 0x07,
		USE_CARD = 0x09,
	};

	enum CODENAME : U32 
	{
		MERONG = 1409286145,
		FAKE_DEATH = 1409286146,
		TUMBLE = 1409286147,
		ROLL_OVER_LEFT = 1409286148,
		ROLL_OVER_RIGHT = 1409286149,
		DOUBLE_JUMP = 1409286150,
		DASH = 1409286151
	};

	CGCard() : GameNetEvent{ CG_CARD, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&uid);
		bstream->read(&targetUid);
		bstream->read(&cmd);
		bstream->read(&cardType);
		bstream->read(&itemId);
		bstream->read(&seqId);
	};

	void handle(GameConnection* conn, Player::Ptr player)
	{
		std::cout << "CGCard::Handle >> Handling Cmd " << cmd << " for UID " << uid << " and TargetUID " << targetUid << " and ItemId " << itemId << " and SeqId " << seqId << "\n";


		if (auto roomPlayer = player->getRoomPlayer(); roomPlayer != nullptr)
		{
			if (auto roomSession = roomPlayer->getRoom()->getRoomSession(); roomSession != nullptr)
			{
				const auto isPublicEnemyGamemode = roomSession->getGameMode()->isPublicEnemyMode();

				if (isPublicEnemyGamemode) 
				{
					const auto character = player->getCharacter();

					if ((itemId == FAKE_DEATH) && (character == 850 || character == 851)) 
					{
						player->broadcast(u"Sorry, you may not use fake death in Public Enemy Mode.");

						return;
					}

					const auto isSearchingForPublicEnemy = roomSession->isSearchingForPublicEnemy();

					if (isSearchingForPublicEnemy && (cmd == 4)) 
					{
						player->broadcast(u"Sorry, you may not perform this action whilst a new public enemy is being selected.");

						return;
					}
				}

				// TODO: Check if the player's character can actually perform the card action.

				roomSession->relayPlaying<GCCard>(uid, targetUid, cmd, cardType, itemId, seqId);
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGCard>(ps);
	};

	U32 uid;
	U32 targetUid;
	U32 cmd;
	U32 cardType;
	U32 itemId;
	U64 seqId;

	TNL_DECLARE_CLASS(CGCard);
};
#endif
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
		CARD_UNK_1 = 3, // Disallow skillcard from activating?
		ACTIVATE_CARD = 4,
		DEACTIVATE_CARD = 9,
		// 10 and 15 both do the same.
		FAIL_CARD_ACTIVATION = 10,
	};

	enum CardType : U32
	{
		INVENTORY_SKILL_CARD = 1,
		ACTION_CARD = 7,
		SKILL_CARD = 9,
	};

	CGCard() : GameNetEvent{ CG_CARD, Guaranteed, DirClientToServer } {};

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
		std::cout << "CGCard::handle >> Handling card for player " << uid << " (CMD: " << cmd << ", CardType: " << cardType << ")" << std::endl;
		std::cout << "CGCard::handle >> Information PlayerId " << uid << ", TargetId: " << targetUid << ", ItemId: " << itemId << ", SeqId: " << seqId << std::endl;

		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		const auto roomSession = roomPlayer->getRoom()->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		if (cardType == ACTION_CARD)
		{
			handleActionCard(player, roomPlayer, roomSession);

			return;
		}

		if (cardType == SKILL_CARD || cardType == INVENTORY_SKILL_CARD)
		{
			handleSkillCard(roomPlayer, roomSession);
		}
	}

	void handleSkillCard(const std::shared_ptr<RoomPlayer>& roomPlayer, const std::shared_ptr<RoomSession>& roomSession) const
	{
		/*const auto isSkillValid = roomSession->getSkillManager()->isValidSkill(itemId);
		const auto areSkillsEnabled = roomPlayer->getRoom()->isSkillsEnabled();

		if (!areSkillsEnabled || !isSkillValid)
		{
			return;
		}*/

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		if (cmd == ACTIVATE_CARD)
		{
			if (roomSessionPlayer->isDead())
			{
				return;
			}

			// In other words, is the skill the player wants to activate also the skill that they have drawn.
			// If a player attempts to activate their skillcard whilst they already have a skillcard active, something isn't right.
			//const auto isDrawnSkillCard = roomSessionPlayer->getSkillManager()->isDrawnSkillCard(itemId);

			//if (const auto playerHasActiveSkillCard = roomSessionPlayer->getSkillManager()->hasActiveSkillCard(); !isDrawnSkillCard || playerHasActiveSkillCard)
			//{
			//	return;
			//}

			//const auto drawnSkillCard = roomSessionPlayer->getSkillManager()->getDrawnSkillCard();

			//if (const auto skillTarget = roomSessionPlayer->getSkillManager()->getDrawnSkillCard()->getSkillTargetType(); skillTarget != SkillTargetType::SELF)
			//{
			//	const auto targetPlayer = roomSession->find(targetUid);

			//	if (targetPlayer == nullptr)
			//	{
			//		roomSessionPlayer->getSkillManager()->failSkillCard(targetUid, seqId);

			//		return;
			//	}

			//	if (targetPlayer->getWeaponManager()->hasEquippedMachineGun())
			//	{
			//		roomSessionPlayer->getSkillManager()->failSkillCard(targetUid, seqId);

			//		return;
			//	}

			//	const auto targetPlayerHasActiveSkillCard = targetPlayer->getSkillManager()->hasActiveSkillCard();
			//	const auto targetPlayerActiveSkillCard = targetPlayer->getSkillManager()->getActiveSkillCard();

			//	if (targetPlayerHasActiveSkillCard && targetPlayerActiveSkillCard->getSkillRateType() == SkillRateType::RAINBOW)
			//	{
			//		roomSessionPlayer->getSkillManager()->failSkillCard(targetUid, seqId);

			//		return;
			//	}

			//	if (const auto targetPlayershouldReflectSkillCard = targetPlayerHasActiveSkillCard
			//		&& targetPlayerActiveSkillCard->shouldReflectSkillCard(); targetPlayershouldReflectSkillCard
			//		&& (skillTarget == SkillTargetType::ENEMY) && drawnSkillCard->isReflectableSkillCard())
			//	{
			//		roomSessionPlayer->getSkillManager()->activateSkillCard(roomSessionPlayer->getPlayer()->getId(), seqId);
			//		return;
			//	}

			//	// TODO: Quality checks to ensure the target player matches the skillcard target type.
			//}

			//if (roomSessionPlayer->getWeaponManager()->hasEquippedMachineGun())
			//{
			//	roomSessionPlayer->getSkillManager()->failSkillCard(targetUid, seqId);

			//	return;
			//}

			//roomSessionPlayer->getSkillManager()->activateSkillCard(targetUid, seqId);
			roomSession->relayPlaying<GCCard>(roomSessionPlayer->getPlayer()->getId(), targetUid, 3, cardType, itemId, seqId, std::vector<uint32_t>{});
			roomSessionPlayer->getSkillManager()->addSkillPoints(0);

			return;
		}
	}

	void handleActionCard(const Player::Ptr& player, const std::shared_ptr<RoomPlayer>& roomPlayer, const std::shared_ptr<RoomSession>& roomSession) const
	{
		if (roomSession->getGameMode()->isPublicEnemyMode())
		{
			if (const auto character = player->getCharacter(); (itemId == ACTION_CARD_FAKE_DEATH) && (character == 850 || character == 851))
			{
				player->broadcast(u"Sorry, you may not use fake death in the public enemy gamemode.");

				return;
			}

			if (const auto isSearchingForPublicEnemy = roomSession->isSearchingForNextTag(); isSearchingForPublicEnemy && (cmd == ACTIVATE_CARD))
			{
				player->broadcast(u"Sorry, you may not perform this action whilst the next public enemy is being selected.");

				return;
			}
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		if (roomSessionPlayer->isDead())
		{
			return;
		}

		// TODO: Check if the player's character can actually perform the card action.
		roomSession->relayPlaying<GCCard>(uid, targetUid, cmd, cardType, itemId, seqId);

		if (roomSessionPlayer->getSkillManager()->hasActiveSkillCard())
		{
			const auto shouldDisableOnRollAction = roomSessionPlayer->getSkillManager()->getActiveSkillCard()->shouldDisableOnRollAction();
			const auto isRollAction = (itemId == ACTION_CARD_ROLL_OVER_LEFT)
				|| (itemId == ACTION_CARD_ROLL_OVER_RIGHT)
				|| (itemId == ACTION_CARD_DASH)
				|| (itemId == ACTION_CARD_TUMBLE);

			if (shouldDisableOnRollAction && isRollAction)
			{
				roomSessionPlayer->getSkillManager()->deactivateSkillCard();
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
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
		CARD_UNK_1 = 3,
		ACTIVATE_CARD = 4,
		DEACTIVATE_CARD = 9,
		// 10 and 15 both do the same.
		FAIL_CARD_ACTIVATION = 10,
	};

	enum CardType : U32
	{
		ACTION_CARD = 7,
		SKILL_CARD = 9,
	};

	enum ActionCardId : U32
	{
		// TODO: Move these item id's to more generic place for item ids.
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

		if (cardType == CardType::ACTION_CARD)
		{
			handleActionCard(player, roomPlayer, roomSession);

			return;
		}

		if (cardType == CardType::SKILL_CARD)
		{
			handleSkillCard(player, roomPlayer, roomSession);

			return;
		}
	}

	void handleSkillCard(Player::Ptr player, const std::shared_ptr<RoomPlayer> roomPlayer, const std::shared_ptr<RoomSession> roomSession)
	{
		//std::cout << "CGCard::handleSkillCard >> Handling skillcard for player " << player->getId() << "." << std::endl;
		//std::cout << "CGCard::handleSkillCard >> Information PlayerId " << player->getId() << ", TargetId: " << targetUid << ", ItemId: " << itemId << ", SeqId: " << seqId << "." << std::endl;

		const auto isSkillValid = roomSession->getSkillManager()->isValidSkill(itemId);
		const auto areSkillsEnabled = roomPlayer->getRoom()->isSkillsEnabled();

		if (!areSkillsEnabled || !isSkillValid)
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		if (cmd == CMD::ACTIVATE_CARD)
		{
			if (roomSessionPlayer->isDead())
			{
				return;
			}

			// In other words, is the skill the player wants to activate also the skill that they have drawn.
			// If a player attempts to activate their skillcard whilst they already have a skillcard active, something isn't right.
			const auto isDrawnSkillCard = roomSessionPlayer->getSkillManager()->isDrawnSkillCard(itemId);

			const auto playerHasActiveSkillCard = roomSessionPlayer->getSkillManager()->hasActiveSkillCard();

			if (!isDrawnSkillCard || playerHasActiveSkillCard)
			{
				return;
			}

			const auto drawnSkillCard = roomSessionPlayer->getSkillManager()->getDrawnSkillCard();

			const auto skillTarget = roomSessionPlayer->getSkillManager()->getDrawnSkillCard()->getSkillTarget();

			if (skillTarget != SkillTargetType::SELF)
			{
				const auto targetPlayer = roomSession->find(targetUid);

				if (targetPlayer == nullptr)
				{
					roomSessionPlayer->getSkillManager()->failSkillCard(targetUid, seqId);

					return;
				}

				if (targetPlayer->getWeaponManager()->hasEquippedMachineGun())
				{
					roomSessionPlayer->getSkillManager()->failSkillCard(targetUid, seqId);

					return;
				}

				const auto targetPlayerHasActiveSkillCard = targetPlayer->getSkillManager()->hasActiveSkillCard();
				const auto targetPlayerActiveSkillCard = targetPlayer->getSkillManager()->getActiveSkillCard();

				if (targetPlayerHasActiveSkillCard && targetPlayerActiveSkillCard->getSkillRateType() == SkillRateType::RAINBOW)
				{
					roomSessionPlayer->getSkillManager()->failSkillCard(targetUid, seqId);

					return;
				}

				const auto targetPlayershouldReflectSkillCard = targetPlayerHasActiveSkillCard && targetPlayerActiveSkillCard->shouldReflectSkillCard();

				if (targetPlayershouldReflectSkillCard && (skillTarget == SkillTargetType::ENEMY) && drawnSkillCard->isReflectableSkillCard())
				{
					roomSessionPlayer->getSkillManager()->activateSkillCard(roomSessionPlayer->getPlayer()->getId(), seqId);
					
					//targetPlayer->getSkillManager()->deactivateSkillCard();

					return;
				}

				// TODO: Quality checks to ensure the target player matches the skillcard target type.
			}

			if (roomSessionPlayer->getWeaponManager()->hasEquippedMachineGun())
			{
				roomSessionPlayer->getSkillManager()->failSkillCard(targetUid, seqId);

				return;
			}

			roomSessionPlayer->getSkillManager()->activateSkillCard(targetUid, seqId);

			return;
		}
	}

	void handleActionCard(Player::Ptr player, const std::shared_ptr<RoomPlayer> roomPlayer, const std::shared_ptr<RoomSession> roomSession)
	{
		const auto isPublicEnemyGamemode = roomSession->getGameMode()->isPublicEnemyMode();

		if (isPublicEnemyGamemode)
		{
			const auto character = player->getCharacter();

			if ((itemId == ActionCardId::FAKE_DEATH) && (character == 850 || character == 851))
			{
				player->broadcast(u"Sorry, you may not use fake death in the public enemy gamemode.");

				return;
			}

			const auto isSearchingForPublicEnemy = roomSession->isSearchingForNextTag();

			if (isSearchingForPublicEnemy && (cmd == CMD::ACTIVATE_CARD))
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
			const auto isRollAction = (itemId == ActionCardId::ROLL_OVER_LEFT)
				|| (itemId == ActionCardId::ROLL_OVER_RIGHT)
				|| (itemId == ActionCardId::DASH)
				|| (itemId == ActionCardId::TUMBLE);

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
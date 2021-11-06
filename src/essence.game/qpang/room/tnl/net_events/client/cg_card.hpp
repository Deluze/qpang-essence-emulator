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
	CGCard() : GameNetEvent{ CG_CARD, Guaranteed, DirClientToServer } {}

	enum Cmd : U32
	{
		DENY_CARD_ACTIVATION = 3,
		ACTIVATE_CARD = 4,
		DEACTIVATE_CARD = 9,
		FAIL_CARD_ACTIVATION = 10/* and 15 */,
	};

	enum CardType : U32
	{
		INVENTORY_SKILL_CARD = 1,
		ACTION_CARD = 7,
		DRAWN_SKILL_CARD = 9,
	};

	void pack(EventConnection* conn, BitStream* bstream) {}
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&uid);
		bstream->read(&targetUid);
		bstream->read(&cmd);
		bstream->read(&cardType);
		bstream->read(&itemId);
		bstream->read(&seqId);
	}

	void handle(GameConnection* conn, const Player::Ptr player)
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

		if (cardType == ACTION_CARD)
		{
			handleActionCard(player, roomPlayer, roomSession);

			return;
		}

		if (cardType == DRAWN_SKILL_CARD || cardType == INVENTORY_SKILL_CARD)
		{
			handleSkillCard(roomPlayer, roomSession);
		}
	}

	void handleSkillCard(const std::shared_ptr<RoomPlayer>& roomPlayer, const std::shared_ptr<RoomSession>& roomSession) const
	{
		if (const auto areSkillsEnabled = roomPlayer->getRoom()->isSkillsEnabled(); !areSkillsEnabled)
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr || roomSessionPlayer->isDead() || cmd != ACTIVATE_CARD)
		{
			return;
		}

		// Check if player already has a skillcard activated.
		if (roomSessionPlayer->getSkillManager()->hasActiveSkill())
		{
			return;
		}

		const auto isValidSkillForGameMode = roomSession->getSkillManager()->isValidSkillForGameMode(itemId);
		const auto isDrawnOrEquippedSkillCard = roomSessionPlayer->getSkillManager()->isDrawnOrEquippedSkill(itemId);

		if (!isValidSkillForGameMode || !isDrawnOrEquippedSkillCard)
		{
			return;
		}

		const auto& skill =
			(cardType == INVENTORY_SKILL_CARD) ? roomSessionPlayer->getSkillManager()->getEquippedSkill(seqId) :
			(cardType == DRAWN_SKILL_CARD) ? roomSessionPlayer->getSkillManager()->getDrawnSkill() :
			nullptr;

		if (skill == nullptr)
		{
			return;
		}

		// Check if player has sufficient skill points.
		if (!roomSessionPlayer->getSkillManager()->hasSufficientSkillPoints(skill))
		{
			return;
		}

		if (!skill->hasUsesLeft())
		{
			return;
		}

		if (roomSessionPlayer->getWeaponManager()->hasEquippedMachineGun())
		{
			roomSessionPlayer->getSkillManager()->failSkill(skill, targetUid, seqId, cardType);

			return;
		}

		const auto skillTargetPlayer = roomSession->find(targetUid);

		if (skillTargetPlayer == nullptr)
		{
			roomSessionPlayer->getSkillManager()->failSkill(skill, targetUid, seqId, cardType);

			return;
		}

		if (skillTargetPlayer->getWeaponManager()->hasEquippedMachineGun())
		{
			roomSessionPlayer->getSkillManager()->failSkill(skill, targetUid, seqId, cardType);

			return;
		}

		if (skillTargetPlayer->getSkillManager()->hasActiveSkill())
		{
			const auto skillTargetPlayerActiveSkillCard = skillTargetPlayer->getSkillManager()->getActiveSkill();

			// TODO: For now, we disallow any skills from being casted on an enemy that has a rainbow skillcard active.
			if (skillTargetPlayerActiveSkillCard->getSkillRateType() == SkillRateType::RAINBOW)
			{
				roomSessionPlayer->getSkillManager()->failSkill(skill, targetUid, seqId, cardType);

				return;
			}

			if (skillTargetPlayerActiveSkillCard->shouldReflectSkillCard() && skill->isReflectableSkillCard()
				&& skill->getSkillTargetType() == SkillTargetType::ENEMY)
			{
				const auto targetPlayerId = roomSessionPlayer->getPlayer()->getId();

				roomSessionPlayer->getSkillManager()->activateSkill(skill, targetPlayerId, seqId, cardType);

				return;
			}
		}

		roomSessionPlayer->getSkillManager()->activateSkill(skill, targetUid, seqId, cardType);
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

		if (roomSessionPlayer->getSkillManager()->hasActiveSkill())
		{
			const auto shouldDisableOnRollAction = roomSessionPlayer->getSkillManager()->getActiveSkill()->shouldDisableOnRollAction();
			const auto isRollAction = (itemId == ACTION_CARD_ROLL_OVER_LEFT)
				|| (itemId == ACTION_CARD_ROLL_OVER_RIGHT)
				|| (itemId == ACTION_CARD_DASH)
				|| (itemId == ACTION_CARD_TUMBLE);

			if (shouldDisableOnRollAction && isRollAction)
			{
				roomSessionPlayer->getSkillManager()->deactivateSkill();
			}
		}
	}

	void process(EventConnection* ps)
	{
		post<CGCard>(ps);
	}

	U32 uid;
	U32 targetUid;
	U32 cmd;
	U32 cardType;
	U32 itemId;
	U64 seqId;

	TNL_DECLARE_CLASS(CGCard);
};
#endif
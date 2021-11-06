#ifndef CC_USER_INFO_H
#define CC_USER_INFO_H

#include "qpang/room/tnl/net_events/GameNetEvent.h"

class CCUserInfo : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CCUserInfo() : GameNetEvent{ CC_USERINFO, GuaranteedOrdered, DirAny } {}

	CCUserInfo(const RoomSessionPlayer::Ptr& roomSessionPlayer) : GameNetEvent{ CC_USERINFO, GuaranteedOrdered, DirAny }
	{
		const auto player = roomSessionPlayer->getPlayer();

		const auto equippedSkillCards = player->getEquipmentManager()->getEquippedSkillCards();
		const auto equippedWeapons = roomSessionPlayer->getWeaponManager()->getWeaponIds();

		this->cmd = 3;

		this->playerId = player->getId();
		this->dataPlayerId = this->playerId;

		this->skillCard1 = equippedSkillCards[0].itemId;
		this->skillCard2 = equippedSkillCards[1].itemId;
		this->skillCard3 = equippedSkillCards[2].itemId;

		const auto activeSkillCard = roomSessionPlayer->getSkillManager()->getActiveSkill();
		const auto activeSkillCardItemId = (activeSkillCard == nullptr) ? 0 : activeSkillCard->getItemId();

		this->skillCard4 = activeSkillCardItemId;

		this->meleeWeapon = equippedWeapons[3];
		this->gunWeapon = equippedWeapons[0];
		this->launcherWeapon = equippedWeapons[1];
		this->throwingWeapon = equippedWeapons[2];

		this->unk_05 = roomSessionPlayer->getHealth();
	}

	U32 cmd = 0; // 1 = unk, 2 = unk, 3 = sets data
	U32 playerId = 0;
	U16 unk_03 = 0;
	U8 unk_04 = 0; // selects melee as weapon?
	U16 unk_05 = 0; // hp decrease?
	U8 unk_06 = 0; // sets a numberbox
	U32 skillCard1 = 0;
	U32 skillCard2 = 0;
	U32 skillCard3 = 0;
	U32 skillCard4 = 0; // don't use; reserved for skill books
	U32 meleeWeapon = 0;
	U32 gunWeapon = 0;
	U32 launcherWeapon = 0;
	U32 throwingWeapon = 0;
	U8 unk_15 = 0; // something with essence
	U32 dataPlayerId = 0;
	U8 unk_17 = 0; // 1 = crash

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(cmd);
		bstream->write(playerId);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(unk_05);
		bstream->write(unk_06);
		bstream->write(skillCard1);
		bstream->write(skillCard2);
		bstream->write(skillCard3);
		bstream->write(skillCard4);
		bstream->write(meleeWeapon);
		bstream->write(gunWeapon);
		bstream->write(launcherWeapon);
		bstream->write(throwingWeapon);
		bstream->write(unk_15);
		bstream->write(dataPlayerId);
		bstream->write(unk_17);
	}

	void unpack(EventConnection* conn, BitStream* bstream)
	{
	}

	void process(EventConnection* conn)
	{
	}

	TNL_DECLARE_CLASS(CCUserInfo);
};

#endif
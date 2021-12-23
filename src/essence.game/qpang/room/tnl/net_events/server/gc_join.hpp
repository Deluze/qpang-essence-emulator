#ifndef GC_JOIN_HPP
#define GC_JOIN_HPP

#include "GameNetEvent.h"

#include "qpang/room/RoomPlayer.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

class GCJoin : public GameNetEvent
{
	typedef NetEvent Parent;

public:
	GCJoin() : GameNetEvent{ GC_JOIN, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient } {};
	GCJoin(RoomPlayer::Ptr roomPlayer, bool spectatorMode = false) : GameNetEvent{ GC_JOIN, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		auto player = roomPlayer->getPlayer();

		this->nickname = player->getName();

		this->state = 0x20;
		this->playerId = player->getId();
		this->characterId = player->getCharacter();
		this->selectedWeapon = player->getEquipmentManager()->getDefaultWeapon();

		this->team = roomPlayer->getTeam();
		this->ready = roomPlayer->isReady();
		this->weaponCount = player->getEquipmentManager()->getWeaponsByCharacter(this->characterId).size();
		this->weapons = player->getEquipmentManager()->getWeaponItemIdsByCharacter(this->characterId);
		this->armor = player->getEquipmentManager()->getArmorItemIdsByCharacter(this->characterId);
		this->playerRank = spectatorMode ? 3 : player->getRank();
		this->refers = player->getPrestige();
		this->level = player->getLevel();
		this->life = player->getEquipmentManager()->getBaseHealth() + player->getEquipmentManager()->getBonusHealth();
		this->experience = player->getExperience();

		if (roomPlayer->getRoom()->getMode() == GameMode::PVE)
			this->actionId = player->getCoins();
	}

	GCJoin(RoomSessionPlayer::Ptr session, bool spectatorMode = false) : GameNetEvent{ GC_JOIN, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		auto player = session->getPlayer();

		this->nickname = player->getName();

		this->state = 0x20;
		this->playerId = player->getId();
		this->characterId = session->getCharacter();
		this->selectedWeapon = session->getWeaponManager()->getSelectedWeapon().itemId;

		this->team = session->getTeam();
		this->ready = true;
		this->weaponCount = session->getWeaponManager()->getWeapons().size();
		this->weapons = session->getWeaponManager()->getWeaponIds();
		this->armor = session->getArmor();
		this->playerRank = spectatorMode && player->getRank() == 3 && !player->getRoomPlayer()->getRoom()->isEventRoom() ? 4 : player->getRank();
		this->refers = player->getPrestige();
		this->level = player->getLevel();
		this->life = session->getDefaultHealth();
		this->experience = player->getExperience();

		if (session->getRoomSession()->getRoom()->getMode() == GameMode::PVE)
			this->actionId = player->getCoins();
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(state); //88 //either 20h, 40h, 80h
		bstream->write(playerId); //92
		bstream->write(characterId); //130
		bstream->write(selectedWeapon); //168
		bstream->write(unk_05); //244
		bstream->write(team); //240 //1: blue, 2: yellow
		bstream->write(ready); //241
		bstream->write(weaponCount); //172 //weapon count (not more than 15??)
		for (U32 item : armor)
			bstream->write(item);

		for (U32 item : weapons)
			bstream->write(U32(item));

		writeByteBuffer(bstream, nickname, 16);
		bstream->write(unk_10); //270
		bstream->write(unk_11); //271
		bstream->write(level); //252
		bstream->write(actionId); //248
		bstream->write(refers); //256
		bstream->write(life); //258
		bstream->write(playerRank); //260 
		bstream->write(experience); //264
		bstream->write(partnerKey); //268
	};
	void unpack(EventConnection * conn, BitStream * bstream) {};
	void process(EventConnection * ps)
	{
	};
	TNL_DECLARE_CLASS(GCJoin);

	U32 state = 0x20; //88 //either 20h, 40h, 80h
	U32 playerId = 1; //92
	U16 characterId = 333; //130
	U32 selectedWeapon = 0; //168
	U32 unk_05 = 0; //244
	U8 team = 1; //240 //1: blue, 2: yellow
	U8 ready = 0; //241
	U16 weaponCount = 4; //172 //weapon count (not more than 15??)

	std::array<U32, 9> armor;

	std::array<U32, 4> weapons;

	std::u16string nickname;

	U8 unk_10 = 0; //270
	U8 unk_11 = 0; //271
	U32 level = 1; //252
	U32 actionId = 0; //248
	U16 refers = 0; //256
	U16 life = 0; //258
	U8 playerRank = 1; //260 
	U32 experience = 0; //264 CURRENT XP APPARENTLY
	U16 partnerKey = 0; //268
};

#endif // !GC_ARRANGED_ACCEPT_HPP
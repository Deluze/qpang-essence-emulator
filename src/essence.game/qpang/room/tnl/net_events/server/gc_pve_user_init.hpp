#ifndef GC_PVE_USER_INIT_HPP
#define GC_PVE_USER_INIT_HPP

#include "GameNetEvent.h"

class GCPvEUserInit : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 state = 0x20; //88 //either 20h, 40h, 80h
	U32 playerId = 1; //92
	U16 characterId = 333; //130
	U32 selectedWeapon = 0; //168
	U16 weaponCount = 4; //172 //weapon count (not more than 15??)

	std::array<U32, 9> armor;

	std::array<U32, 4> weapons;

	std::u16string nickname;

	U8 playerRank = 1; // 240
	U32 coins = 0; // 244
	U16 doubleCoins = 0; // 248 anything else than 0 enables double coins for when the person picks up coins

	GCPvEUserInit() : GameNetEvent{ GC_PVE_USER_INIT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirServerToClient } {};
	GCPvEUserInit(RoomPlayer::Ptr roomPlayer, bool spectatorMode = false) : GameNetEvent{ GC_PVE_USER_INIT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		auto player = roomPlayer->getPlayer();

		this->nickname = player->getName();

		this->state = 0x20;
		this->playerId = player->getId();
		this->characterId = player->getCharacter();
		this->selectedWeapon = player->getEquipmentManager()->getDefaultWeapon();
		this->weaponCount = player->getEquipmentManager()->getWeaponsByCharacter(this->characterId).size();
		this->weapons = player->getEquipmentManager()->getWeaponItemIdsByCharacter(this->characterId);
		this->armor = player->getEquipmentManager()->getArmorItemIdsByCharacter(this->characterId);
		this->playerRank = spectatorMode ? 3 : player->getRank();

		this->coins = player->getCoins();
	}

	GCPvEUserInit(RoomSessionPlayer::Ptr session, bool spectatorMode = false) : GameNetEvent{ GC_PVE_USER_INIT, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		auto player = session->getPlayer();

		this->nickname = player->getName();

		this->state = 0x20;
		this->playerId = player->getId();
		this->characterId = session->getCharacter();
		this->selectedWeapon = session->getWeaponManager()->getSelectedWeapon().itemId;

		this->weaponCount = session->getWeaponManager()->getWeapons().size();
		this->weapons = session->getWeaponManager()->getWeaponIds();
		this->armor = session->getArmor();
		this->playerRank = spectatorMode && player->getRank() == 3 && !player->getRoomPlayer()->getRoom()->isEventRoom() ? 4 : player->getRank();
	
		this->coins = player->getCoins();
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(state); //88 //either 20h, 40h, 80h
		bstream->write(playerId); //92
		bstream->write(characterId); //130
		bstream->write(selectedWeapon); //168
		bstream->write(weaponCount); //172

		for (U32 item : armor)
			bstream->write(item);

		for (U32 item : weapons)
			bstream->write(U32(item));

		writeByteBuffer(bstream, nickname, 16);
		bstream->write(playerRank); //240
		bstream->write(coins); //244
		bstream->write(doubleCoins); //248
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};

	void process(EventConnection* ps)
	{
	};

	TNL_DECLARE_CLASS(GCPvEUserInit);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
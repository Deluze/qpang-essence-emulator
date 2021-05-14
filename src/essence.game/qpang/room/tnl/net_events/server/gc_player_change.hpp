#ifndef GC_PLAYER_CHANGE_HPP
#define GC_PLAYER_CHANGE_HPP

#include "GameNetEvent.h"
#include <array>

class GCPlayerChange : public GameNetEvent
{
	typedef NetEvent Parent;

public:
	GCPlayerChange() : GameNetEvent{ GC_PLAYER_CHANGE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCPlayerChange(Player::Ptr player, U8 cmd, U32 value) : GameNetEvent{ GC_PLAYER_CHANGE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->playerId = player->getId();
		this->cmd = cmd;
		this->value = value;
		this->equipment = player->getEquipmentManager()->getArmorItemIdsByCharacter(player->getCharacter());
	};
	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(playerId);
		bstream->write(cmd);
		bstream->write(value);

		for (size_t i = 0; i < 9; i++)
			bstream->write(U32(equipment.at(i)));
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 playerId;
	U8 cmd;
	U32 value;

	std::array<uint32_t, 9> equipment;

	TNL_DECLARE_CLASS(GCPlayerChange);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
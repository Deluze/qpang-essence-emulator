#ifndef GC_ROOM_HPP
#define GC_ROOM_HPP

#include <string>
#include <cstdint>

#include "GameNetEvent.h"

#include "qpang/room/Room.h"

class GCRoom : public GameNetEvent
{
	typedef NetEvent Parent;

public:
	GCRoom() : GameNetEvent{ GC_ROOM, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	GCRoom(uint32_t playerId, uint32_t cmd, Room::Ptr room) : GameNetEvent{ GC_ROOM, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->zero = 0;
		this->playerId = playerId;
		this->cmd = cmd;
		this->roomId = room->getId();
		this->mode = room->getMode();
		this->memberCount = room->getPlayerCount();
		this->title = room->getName();
		this->meleeOnly = room->isMeleeOnly();
		this->skillsEnabled = room->isSkillsEnabled();
	};

	GCRoom(uint32_t playerId, uint32_t cmd, uint32_t val, Room::Ptr room) : GameNetEvent{ GC_ROOM, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->zero = 0;
		this->playerId = playerId;
		this->cmd = cmd;
		this->value = val;
		this->mode = room->getMode();
		this->meleeOnly = room->isMeleeOnly();
		this->title = u"-";
		this->password = room->getPassword();
		this->skillsEnabled = room->isSkillsEnabled();
	};

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(playerId);
		bstream->write(cmd);
		bstream->write(value);

		bstream->write(mode);
		bstream->write(memberCount);
		bstream->write(goal);

		bstream->writeString(password.c_str(), 255);
		writeByteBuffer(bstream, title);
		bstream->write(time);
		bstream->write(rounds);
		bstream->write(_160);

		bstream->write(zero);

		bstream->write(_161);
		bstream->write(skillsEnabled);
		bstream->write(meleeOnly);
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};


	U32 playerId; //92
	U32 cmd; //96

	union
	{
		U32 value; //100
		U32 roomId; //100
	};


	U8 mode; //109
	U8 memberCount; //110
	U8 goal = 10; //111

	std::string password;
	std::u16string title;
	U8 time = 6; //154

	union
	{
		U8 rounds = 1; //155 
		U8 gameId; //might be u8
	};

	U8 _160 = 1; //156 //In CGRoom

	U32 zero = 0; //set to 0
	U8 _161 = 0; //164
	U8 skillsEnabled = 0; //165
	U8 meleeOnly = 0; //166



	TNL_DECLARE_CLASS(GCRoom);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
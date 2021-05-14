#ifndef GC_ROOM_INFO_HPP
#define GC_ROOM_INFO_HPP

#include "GameNetEvent.h"

#include "qpang/room/Room.h"

class GCRoomInfo : public GameNetEvent
{
	typedef NetEvent Parent;

public:
	GCRoomInfo() : GameNetEvent{ GC_ROOM_INFO, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCRoomInfo(Room::Ptr room, U8 spectating = 0) : GameNetEvent{ GC_ROOM_INFO, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->masterUid = room->getMasterId();
		this->joinNum = room->getMaxPlayers();
		this->gameMode = room->getMode();
		this->goalPoint = room->isPointsGame() ? room->getScorePoints() : room->getScoreTime();
		this->mapNum = room->getMap();
		this->roomState = room->getState();
		this->respawnTime = 10 * 1000;
		this->p2pWaitTime = 6000;
		this->title = room->getName();
		this->isTime = !room->isPointsGame();
		this->rounds = 0;
		this->levelLimit = false;
		this->gameId = 10;
		this->teamBalance = room->isTeamSorting();
		this->skillMode = room->isSkillsEnabled();
		this->pingLevel = NULL;
		this->melee = room->isMeleeOnly();
		this->event = room->isEventRoom() || spectating;
	}
	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(masterUid); //88
		bstream->write(joinNum); //134 
		bstream->write(gameMode); //136 
		bstream->write(goalPoint); //140
		bstream->write(mapNum); //144
		bstream->write(roomState); //148
		bstream->write(respawnTime); //152
		bstream->write(p2pWaitTime); //154 
		writeByteBuffer(bstream, title);
		bstream->write(isTime); //156
		bstream->write(rounds); //157
		bstream->write(levelLimit); //164
		bstream->write(gameId); //160
		bstream->write(teamBalance); //165
		bstream->write(skillMode); //166
		bstream->write(pingLevel); //167
		bstream->write(melee); //168
		bstream->write(event); //169
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 masterUid; //88
	U16 joinNum; //134 
	U16 gameMode; //136 
	U32 goalPoint; //140

	U16 mapNum; //144
	U32 roomState; //148
	U16 respawnTime; //152
	U16 p2pWaitTime; //154 

	std::u16string title; //172

	U8 isTime; //156
	U8 rounds; //157
	U8 levelLimit; //164
	U32 gameId; //160
	U8 teamBalance; //165
	U8 skillMode; //166
	U8 pingLevel; //167
	U8 melee; //168
	U8 event; //169

	TNL_DECLARE_CLASS(GCRoomInfo);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
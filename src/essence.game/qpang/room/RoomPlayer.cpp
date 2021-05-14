#include "RoomPlayer.h"

#include "qpang/room/tnl/GameConnection.h"
#include "qpang/room/tnl/net_events/GameNetEvent.h"

#include "qpang/room/tnl/net_events/server/gc_ready.hpp"
#include "qpang/room/tnl/net_events/server/gc_player_change.hpp"

RoomPlayer::RoomPlayer(GameConnection* conn, std::shared_ptr<Room> room) :
	m_conn(conn),
	m_room(room),
	m_team(0),
	m_isReady(false),
	m_isPlaying(false),
	m_isSpectating(false)
{
	assert(conn->getPlayer() != nullptr);

	m_conn->incRef();
}

RoomPlayer::~RoomPlayer()
{
	m_conn->decRef();
}

std::shared_ptr<Player> RoomPlayer::getPlayer()
{
	return m_conn->getPlayer();
}

std::shared_ptr<Room> RoomPlayer::getRoom()
{
	return m_room;
}

GameConnection* RoomPlayer::getConnection()
{
	return m_conn;
}

void RoomPlayer::send(GameNetEvent* evt)
{
	m_conn->postNetEvent(evt);
}

uint8_t RoomPlayer::getTeam()
{
	return m_team;
}

void RoomPlayer::setTeam(uint8_t team)
{
	m_team = team;

	m_room->broadcastWaiting<GCPlayerChange>(getPlayer(), 2, team);
}

bool RoomPlayer::isReady()
{
	return m_isReady;
}

void RoomPlayer::setReady(bool isReady)
{
	m_isReady = isReady;

	m_room->broadcastWaiting<GCReady>(m_conn->getPlayer()->getId(), isReady);
}

bool RoomPlayer::isPlaying()
{
	return m_isPlaying;
}

void RoomPlayer::setPlaying(bool isPlaying)
{
	m_isPlaying = isPlaying;
}

bool RoomPlayer::isSpectating()
{
	return m_isSpectating;
}

void RoomPlayer::setSpectating(bool isSpectating)
{
	m_isSpectating = isSpectating;
}

void RoomPlayer::setRoomSessionPlayer(std::shared_ptr<RoomSessionPlayer> player)
{
	m_roomSessionPlayer = player;
}

void RoomPlayer::onStart()
{
	getPlayer()->getEquipmentManager()->save();
}

std::shared_ptr<RoomSessionPlayer> RoomPlayer::getRoomSessionPlayer()
{
	return m_roomSessionPlayer.lock();
}

#include "RoomPlayer.h"

#include "qpang/room/tnl/GameConnection.h"
#include "qpang/room/tnl/net_events/GameNetEvent.h"

#include "qpang/room/tnl/net_events/server/gc_ready.hpp"
#include "qpang/room/tnl/net_events/server/gc_player_change.hpp"

RoomPlayer::RoomPlayer(GameConnection* conn, const std::shared_ptr<Room> room) :
	m_team(0),
	m_isReady(false),
	m_isPlaying(false),
	m_isSpectating(false),
	m_conn(conn),
	m_room(room)
{
	assert(conn->getPlayer() != nullptr);

	m_conn->incRef();
}

RoomPlayer::~RoomPlayer()
{
	m_conn->decRef();
}

std::shared_ptr<Player> RoomPlayer::getPlayer() const
{
	return m_conn->getPlayer();
}

std::shared_ptr<Room> RoomPlayer::getRoom() const
{
	return m_room;
}

GameConnection* RoomPlayer::getConnection() const
{
	return m_conn;
}

void RoomPlayer::send(GameNetEvent* evt) const
{
	m_conn->postNetEvent(evt);
}

uint8_t RoomPlayer::getTeam() const
{
	return m_team;
}

void RoomPlayer::setTeam(const uint8_t team)
{
	m_team = team;

	m_room->broadcastWaiting<GCPlayerChange>(getPlayer(), 2, team);
}

bool RoomPlayer::isReady() const
{
	return m_isReady;
}

void RoomPlayer::setReady(const bool isReady)
{
	m_isReady = isReady;

	m_room->broadcastWaiting<GCReady>(m_conn->getPlayer()->getId(), isReady);
}

bool RoomPlayer::isPlaying() const
{
	return m_isPlaying;
}

void RoomPlayer::setPlaying(const bool isPlaying)
{
	m_isPlaying = isPlaying;
}

bool RoomPlayer::isSpectating() const
{
	return m_isSpectating;
}

void RoomPlayer::setSpectating(const bool isSpectating)
{
	m_isSpectating = isSpectating;
}

void RoomPlayer::setRoomSessionPlayer(const std::shared_ptr<RoomSessionPlayer> player)
{
	m_roomSessionPlayer = player;
}

void RoomPlayer::onStart() const
{
	getPlayer()->getEquipmentManager()->save();
}

std::shared_ptr<RoomSessionPlayer> RoomPlayer::getRoomSessionPlayer() const
{
	return m_roomSessionPlayer.lock();
}

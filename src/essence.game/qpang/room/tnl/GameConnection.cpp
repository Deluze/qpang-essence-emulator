#include "GameConnection.h"

#include <iostream>

#include <tnl/tnlNetInterface.h>

#include "qpang/Game.h"
#include "qpang/room/RoomServer.h"
#include "qpang/player/Player.h"
#include "qpang/room/Room.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_room.hpp"
#include "qpang/room/tnl/net_events/server/gc_room_info.hpp"
#include "qpang/room/tnl/net_events/server/gc_join.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include "qpang/room/tnl/net_events/server/gc_start.hpp"
#include "qpang/room/tnl/net_events/server/gc_player_change.hpp"
#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_arranged_accept.hpp"

#include "qpang/room/tnl/net_events/server/gc_pve_start.hpp"
#include "qpang/room/tnl/net_events/server/gc_pve_user_init.hpp"
#include "qpang/room/tnl/net_events/server/gc_pve_new_round.hpp"

TNL_IMPLEMENT_NETCONNECTION(GameConnection, TNL::NetClassGroupGame, true);

GameConnection::GameConnection() : TNL::EventConnection()
{

/// #------------------------------------------------------------------------------ 	
/// 
/// ## UDP��Ʈ��ũ Ÿ�Ӿƿ�˻� 	
/// 
/// #------------------------------------------------------------------------------ 	
/// 
/// 	pingTimeout = 5000	# �и��� ����
/// 
/// 	pingRetryCnt = 10 	# �ִ� �˻� Ƚ��
	setPingTimeouts(5000, 10);
///#------------------------------------------------------------------------------ 	
///
///## flowControl�� auto off �ϸ� ����ϴ� �� 	
///
///#------------------------------------------------------------------------------ 	
///
///	flowControl = 1 # auto = 1, manual = 0, auto�� ��� �Ʒ� ������ ������� �ʴ´�.
///
///	minSendPeriod = 50		# Minimum millisecond delay(maximum rate) between packet sends
///
///	minRecvPeriod = 50 		# Minimum millisecond delay the remote host should allow between sends
///
///	maxSendBandwidth = 1000		# Number of bytes per second we can send over the connection.
///
///	maxRecvBandwidth = 1000		# Number of bytes per second max that the remote instance should send.
///
	setFixedRateParameters(50, 50, 1000, 1000);

	setIsConnectionToClient();
}

void GameConnection::setPlayer(std::shared_ptr<Player> player)
{
	m_player = player;
}

std::shared_ptr<Player> GameConnection::getPlayer()
{
	return m_player;
}

void GameConnection::onConnectionEstablished()
{
	setIsConnectionToClient();
}

void GameConnection::onConnectionTerminated(TNL::NetConnection::TerminationReason reason, const char* msg)
{
	if (m_player == nullptr)
		return;

	try
	{
		Game::instance()->getRoomServer()->dropConnection(m_player->getId());
	}
	catch (const std::exception& e)
	{
		std::cout << "An exception occurred: " << e.what() << std::endl;
	}
}

void GameConnection::onConnectTerminated(TNL::NetConnection::TerminationReason reason, const char* msg)
{
	if (m_player == nullptr)
		return;

	try
	{
		Game::instance()->getRoomServer()->dropConnection(m_player->getId());
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void GameConnection::enterRoom(std::shared_ptr<Room> room)
{
	postNetEvent(new GCRoom(m_player->getId(), 9, room));
	postNetEvent(new GCRoomInfo(room));

	updateRoom(room, room->isPointsGame() ? 4 : 20, room->isPointsGame() ? room->getScorePoints() : room->getScoreTime());
	updateRoom(room, 26, false);
}

void GameConnection::updateRoom(std::shared_ptr<Room> room, TNL::U32 cmd, TNL::U32 val)
{
	postNetEvent(new GCRoom(m_player->getId(), cmd, val, room));
}

void GameConnection::startLoadingPve(std::shared_ptr<Room> room, RoomPlayer::Ptr roomPlayer)
{
	/*postNetEvent(new GCStart(room, m_player->getId()));
	postNetEvent(new GCJoin(roomPlayer));
	postNetEvent(new GCGameState(m_player->getId(), 12));*/

	postNetEvent(new GCPvEStart(room, m_player->getId()));
	postNetEvent(new GCPvEUserInit(roomPlayer));
	postNetEvent(new GCGameState(m_player->getId(), 13));

	//postNetEvent(new GCJoin(roomPlayer));
//	postNetEvent(new GCPvENewRound(m_player->getId()));

	/*postNetEvent(new GCPvEStart(room, m_player->getId()));
	postNetEvent(new GCPvEUserInit(roomPlayer));
	postNetEvent(new GCGameState(m_player->getId(), 12));
	postNetEvent(new GCPvENewRound());*/

	//startLoading(room, roomPlayer);
}

void GameConnection::startLoading(std::shared_ptr<Room> room, RoomPlayer::Ptr roomPlayer)
{
	postNetEvent(new GCStart(room, m_player->getId()));
	postNetEvent(new GCJoin(roomPlayer));
	postNetEvent(new GCGameState(m_player->getId(), 12));
}

void GameConnection::startSpectating(std::shared_ptr<Room> room, RoomPlayer::Ptr roomPlayer)
{
	postNetEvent(new GCRoomInfo(room, true));
	postNetEvent(new GCStart(room, m_player->getId()));
	postNetEvent(new GCJoin(roomPlayer, true));
	postNetEvent(new GCGameState(m_player->getId(), 12));
}

void GameConnection::startGameButNotReady()
{
	postNetEvent(new GCGameState(m_player->getId(), 3));
	postNetEvent(new GCPlayerChange(m_player, 0, 0));
}

void GameConnection::addSession(RoomSessionPlayer::Ptr session)
{
	postNetEvent(new GCArrangedAccept(m_player->getId(), session->getPlayer()->getId()));
	postNetEvent(new GCJoin(session, m_player->getRoomPlayer()->isSpectating()));
	postNetEvent(new GCRespawn(session->getPlayer()->getId(), session->getCharacter(), 1));
}

void GameConnection::spawnEssence(const Spawn& spawn)
{
	postNetEvent(new GCRespawn(0, 3, 5, spawn.x, spawn.y, spawn.z));
}

void GameConnection::dropEssence(const Spawn& spawn)
{

}

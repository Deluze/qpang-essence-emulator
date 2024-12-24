#pragma once

#include <iostream>

#include <tnl/tnl.h>
#include <tnl/tnlEventConnection.h>

#include "qpang/player/Player.h"
#include "qpang/room/Room.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/spawn/Spawn.h"

class Player;
class Room;
class RoomPlayer;
class RoomSessionPlayer;

class GameConnection : public TNL::EventConnection
{

public:
	GameConnection();

	void onConnectionEstablished() override;
	void onConnectionTerminated(TNL::NetConnection::TerminationReason reason, const char* msg) override;
	void onConnectTerminated(TNL::NetConnection::TerminationReason reason, const char* msg) override;

	void setPlayer(std::shared_ptr<Player> player);
	std::shared_ptr<Player> getPlayer();

	void enterRoom(std::shared_ptr<Room> room);
	void updateRoom(std::shared_ptr<Room> room, TNL::U32 cmd, TNL::U32 val);

	void startLoadingPve(std::shared_ptr<Room> room, std::shared_ptr<RoomPlayer> roomPlayer);

	void startLoading(std::shared_ptr<Room> room, std::shared_ptr<RoomPlayer> roomPlayer);
	void startSpectating(std::shared_ptr<Room> room, std::shared_ptr<RoomPlayer> roomPlayer);
	void startGameButNotReady();

	void addSession(std::shared_ptr<RoomSessionPlayer> session);

	void spawnEssence(const Spawn& spawn);
	void dropEssence(const Spawn& spawn);

	TNL_DECLARE_NETCONNECTION(GameConnection);
private:
	std::shared_ptr<Player> m_player;
};
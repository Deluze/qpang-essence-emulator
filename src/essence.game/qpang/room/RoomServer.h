#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "qpang/room/tnl/GameNetInterface.h"

class GameNetEvent;

class RoomServer
{
public:
	void initialize();
	void run();
	void tick();

	void handleEvent(GameNetEvent* netEvent);
	bool createConnection(uint32_t playerId, GameConnection* connection);
	void dropConnection(uint32_t playerId);
private:
	static void processEvent(GameNetEvent* netEvent);

	bool m_isRunning = false;

	std::recursive_mutex m_connMx;

	std::vector<uint32_t> m_connsToDispose;
	std::unordered_map<uint32_t, GameConnection*> m_connections;

	std::unique_ptr<GameNetInterface> m_netInterface;

	time_t m_lastDisposal = 0;
	time_t m_lastTick = 0;
};
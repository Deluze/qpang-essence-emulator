#include "RoomServer.h"

#include <functional>
#include <iostream>

#include "GameNetEvent.h"

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

#include "qpang/room/tnl/net_events/GameNetEvent.h"
#include "qpang/room/tnl/GameNetInterface.h"

#include "qpang/room/Room.h"
#include "qpang/room/RoomPlayer.h"

void RoomServer::initialize()
{
	m_lastDisposal = time(NULL);

	auto port = static_cast<uint16_t>(CONFIG_MANAGER->getInt("GAME_PORT"));
	TNL::Address a(TransportProtocol::IPProtocol, TNL::Address::NamedAddress::Any, port);

	try
	{
		m_netInterface = std::make_unique<GameNetInterface>(a);
	}
	catch (const std::exception& e)
	{
		std::cout << "[RoomServer::initialize] An exception occurred: " << e.what() << std::endl;
		return;
	}
}

void RoomServer::run()
{
	m_isRunning = true;

	while (m_isRunning)
	{
		m_connMx.lock();

		try
		{
			m_netInterface->processConnections();
			m_netInterface->checkIncomingPackets();
			tick();
		}
		catch (const std::exception& e)
		{
			std::cout << "[RoomServer::run]: " << e.what() << "\n";
		}
		catch (...)
		{
			std::cout << "[RoomServer::run]: Unknown exception.\n";
		}
	
		m_connMx.unlock();
	}
}

void RoomServer::tick()
{
	const auto currTime = time(NULL);
	
	if (m_lastTick < currTime)
	{
		m_lastTick = currTime;
		Game::instance()->getRoomManager()->tick();
	}

	if (m_connsToDispose.empty())
		return;

	if (m_lastDisposal <= currTime - 1)
	{
		m_lastDisposal = currTime;
		
		m_connMx.lock();
		
		for (const auto playerId : m_connsToDispose)
		{
			const auto it = m_connections.find(playerId);

			if (it == m_connections.cend())
				continue;
			
			auto conn = it->second;
			
			if (conn != NULL)
			{
				if (const auto roomPlayer = conn->getPlayer()->getRoomPlayer(); roomPlayer != nullptr)
					roomPlayer->getRoom()->removePlayer(playerId);

				if (conn->getConnectionState() == NetConnection::Connected)
					conn->disconnect("disconnected by server");

				conn->decRef();
			}

			m_connections.erase(playerId);
		}
		
		m_connsToDispose.clear();
		m_connMx.unlock();
	}
}

void RoomServer::handleEvent(GameNetEvent* netEvent)
{
	processEvent(netEvent);
}

bool RoomServer::createConnection(uint32_t playerId, GameConnection* connection)
{
	std::lock_guard<std::recursive_mutex> lg(m_connMx);

	auto player = Game::instance()->getOnlinePlayer(playerId);

	if (player == nullptr)
		return false;

	connection->incRef();
	connection->setPlayer(player);

	m_connections[playerId] = connection;

	return true;
}

void RoomServer::dropConnection(uint32_t playerId)
{
	std::lock_guard<std::recursive_mutex> lg(m_connMx);

	const auto it = m_connections.find(playerId);

	if (it == m_connections.cend())
		return;

	const auto alreadyGettingDisposed = std::find(m_connsToDispose.cbegin(), m_connsToDispose.cend(), playerId) != m_connsToDispose.cend();
	
	if (alreadyGettingDisposed)
		return;

	m_connsToDispose.push_back(playerId);
}

void RoomServer::processEvent(GameNetEvent* netEvent)
{
	netEvent->gameConnection->incRef();

	try
	{		
		const auto isAuthEvent = netEvent->id == GameNetId::CG_AUTH;
		const auto isAuthorized = netEvent->gameConnection->getPlayer() != nullptr;

		if (isAuthorized && !isAuthEvent)
			netEvent->handle(netEvent->gameConnection, netEvent->gameConnection->getPlayer());
		else if (!isAuthorized && isAuthEvent)
			netEvent->handle(netEvent->gameConnection, netEvent->gameConnection->getPlayer());
	}
	catch (const std::exception& e)
	{
		std::cout << "[RoomServer::processEvent]: An exception occurred: " << netEvent->id << " - " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "[RoomServer::processEvent]: An exception occurred: " << netEvent->id << " - Unknown exception.\n";
	}

	netEvent->gameConnection->decRef();
}

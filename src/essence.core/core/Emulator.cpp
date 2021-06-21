#include "Emulator.h"

#include <boost/thread.hpp>

#include "core/communication/server/QpangServer.h"

#include <iostream>
#ifdef WIN32
#include <Windows.h>
#endif

#include "core/database/Database.h"
#include "core/config/ConfigManager.h"

void Emulator::initialize()
{
#ifdef WIN32
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(handle, 13);
#endif
	std::cout << "                   ^		\n"
		<< "                  / \\			\n"
		<< "                 /   \\			\n"
		<< "                /   Essence		\n"
		<< "               /       Emulator	\n"
		<< "              /         \\		\n"
		<< "              \\         /	    \n"
		<< "           By: \\       /   	\n"
		<< "              Dennis &/     	\n"
		<< "                 Deluze     	\n"
		<< "                  \\ /     		\n"
		<< "                   v     		\n \n";
#ifdef WIN32
	SetConsoleTextAttribute(handle, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
#endif

	m_configManager = new ConfigManager();
	m_database = new Database();

	if (!m_configManager->initialize() || !m_database->initialize())
	{
		std::cout << "[Emulator::initialize]: Errors encountered, can not start emulator.\n";
		abort();
	}
}

void Emulator::attachServer(QpangServer* server)
{
	m_servers.push_back(server);
}

void Emulator::run()
{
	m_isRunning = true;

	for (QpangServer* server : m_servers)
		boost::thread t(std::bind(&QpangServer::listen, server));

	std::cout << "[Emulator::run]: Emulator is running.\n";

	auto now = std::chrono::system_clock::now();
	auto nextQueryTime = now + std::chrono::hours(1);

	while (m_isRunning)
	{
		now = std::chrono::system_clock::now();

		if (nextQueryTime < now) {
			const auto statement = DATABASE->prepare("SELECT * FROM qfighter.weapons LIMIT 0;");

			statement->fetch();

			nextQueryTime = now + std::chrono::hours(1);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Emulator::stop()
{
	m_isRunning = false;
}

Database* Emulator::getDatabase()
{
	return m_database;
}

ConfigManager* Emulator::getConfigManager()
{
	return m_configManager;
}

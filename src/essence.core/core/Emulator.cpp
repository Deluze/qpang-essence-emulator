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
	std::cout	<< "                   ^		\n"
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
		std::cout << "Errors encountered, can not start emulator. \n";
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

	std::cout << "Emulator is running! \n";

	while (m_isRunning)
	{
		// do console commands

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

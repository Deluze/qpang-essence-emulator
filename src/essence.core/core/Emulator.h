#pragma once

#include <vector>

class Database;
class ConfigManager;
class QpangServer;

class Emulator
{
public:
	static Emulator* instance()
	{
		static Emulator emulator;
		return &emulator;
	}

	void initialize();
	void attachServer(QpangServer* server);
	void run();
	void stop();

	Database* getDatabase();
	ConfigManager* getConfigManager();
	
private:
	Database* m_database;
	ConfigManager* m_configManager;
	bool m_isRunning;
	std::vector<QpangServer*> m_servers;
};

#define CONFIG_MANAGER Emulator::instance()->getConfigManager()
#define DATABASE Emulator::instance()->getDatabase()
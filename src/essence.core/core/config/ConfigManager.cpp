#include "ConfigManager.h"

#include <iostream>
#include <boost/filesystem.hpp>

#include "core/config/constants/ConfigTemplate.h"

bool ConfigManager::initialize()
{
	std::ifstream file(CONFIG_TEMPLATE_FILE_NAME);

	if (!file.good()) // create file if it doesn't exist
	{
		file.close();

		std::ofstream output(CONFIG_TEMPLATE_FILE_NAME);
		output << CONFIG_TEMPLATE_STRING;
		output.close();

		std::cout << "[ConfigManager::initialize]: Config file generated. Please fill in the server settings and restart the server.\n";

		return false;
	}

	std::cout << "[ConfigManager::initialize]: Parsing configuration file.\n";

	std::string line;

	while (std::getline(file, line))
	{
		if (line.empty() || line.at(0) == '#')
			continue;

		size_t off = line.find('=');

		std::string key = line.substr(0, off);
		std::string value = line.substr(off + 1, line.size() - key.size() + 1);

		set(key, value);

		if (key == "GAME_REVISION") {
			std::cout << "[ConfigManager::initialize]: Game revision: " << value << "\n";
		}
	}

	file.close();

	return true;
}

std::string ConfigManager::getSetting(std::string key)
{
	std::unordered_map<std::string, std::string>::iterator it = m_settings.find(key);

	if (it == m_settings.end())
	{
		std::cout << "[ConfigManager::getSetting]: Config key " << key << " does not exist.\n";

		return "";
	}

	return it->second;
}

std::string ConfigManager::getString(std::string key)
{
	return getSetting(key);
}

uint32_t ConfigManager::getInt(std::string key)
{
	std::string val = getSetting(key);

	if (val.empty())
		return 0;

	return std::stoi(val);
}

void ConfigManager::set(const std::string& key, const std::string& value)
{
	m_settings[key] = value;
}

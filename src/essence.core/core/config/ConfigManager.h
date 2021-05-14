#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

class ConfigManager
{
public:
	bool initialize();
	std::string getString(std::string key);
	uint32_t getInt(std::string key);

	void set(const std::string& key, const std::string& value);
private:
	std::string getSetting(std::string key);
	std::unordered_map<std::string, std::string> m_settings;
};
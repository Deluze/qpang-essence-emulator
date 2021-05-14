#pragma once

#include <unordered_map>
#include <memory>

#include "qpang/channel/Channel.h"
#include "core/database/Database.h"

class ChannelManager
{
public:
	void initialize();
	std::vector<Channel> list();
	Channel getChannel(uint32_t channelId);
private:
	std::unordered_map<uint32_t, Channel> m_channels;
};
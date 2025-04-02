#include "ChannelManager.h"

#include "core/Emulator.h"

void ChannelManager::initialize()
{
	std::cout << "Loading channels.\n";

	Statement::Ptr stmt = Emulator::instance()->getDatabase()->prepare("SELECT * FROM channels");
	StatementResult::Ptr res = stmt->fetch();

	while (res->hasNext())
	{
		Channel ch{
			res->getInt("id"),
			res->getString("name"),
			res->getTiny("min_level"),
			res->getTiny("max_level"),
			res->getShort("max_players"),
			0
		};

		m_channels[ch.id] = ch;
		res->next();
	}

	std::cout << "Loaded " << m_channels.size() << " channels.\n";
}

std::vector<Channel> ChannelManager::list()
{
	std::vector<Channel> channels;

	for (auto [key, val] : m_channels)
	{
		channels.push_back(val);
	}

	return channels;
}

Channel ChannelManager::getChannel(uint32_t channelId)
{
	const auto ch = m_channels.find(channelId);

	if (ch == m_channels.cend())
		return {  };
	
	return ch->second;

}

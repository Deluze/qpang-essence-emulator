#pragma once

#include <qpang/player/Player.h>

class TradeSessionParticipant
{
public:
	TradeSessionParticipant(const Player::Ptr& player)
	{
		m_player = player;
	}

	Player::Ptr getPlayer();
private:
	Player::Ptr m_player;
};
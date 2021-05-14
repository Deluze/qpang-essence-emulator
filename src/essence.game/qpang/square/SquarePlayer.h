#pragma once

#include <memory>
#include <cstdint>
#include <array>
#include <string>

class Player;
class Square;

class SquarePlayer : public std::enable_shared_from_this<SquarePlayer>
{
public:
	using Ptr = std::shared_ptr<SquarePlayer>;
	SquarePlayer(std::shared_ptr<Player> player, std::shared_ptr<Square> square);

	void close();
	void changeWeapon(uint32_t itemId);
	void move(std::array<float, 3> position, uint8_t direction, uint8_t moveType);
	void chat(const std::u16string& message);
	void emote(const uint32_t emoteId);

	uint32_t getSelectedWeapon();

	void setState(uint32_t state, uint8_t stateValue = 0);
	uint32_t getState();
	uint8_t getStateValue();

	std::shared_ptr<Square> getSquare();
	std::shared_ptr<Player> getPlayer();
	std::array<float, 3> getPosition();
private:
	std::shared_ptr<Square> m_square;
	std::shared_ptr<Player> m_player;

	uint32_t m_selectedWeapon;
	uint32_t m_state; // 5 = in-game, 
	uint8_t m_stateValue;
	std::array<float, 3> m_position;
};
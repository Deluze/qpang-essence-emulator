#include "SquarePlayer.h"

#include "qpang/player/Player.h"
#include "qpang/square/Square.h"

#include "packets/square/outgoing/SendSquarePlayerMove.h"
#include "packets/square/outgoing/SendSquareChatMessage.h"
#include "packets/square/outgoing/SendUseEmote.h"
#include "packets/square/outgoing/SendUpdateSquarePlayerState.h"
#include "packets/square/outgoing/SendAddSquarePlayer.h"
#include "packets/square/outgoing/SendUpdatePlayerEquipment.h"

SquarePlayer::SquarePlayer(std::shared_ptr<Player> player, std::shared_ptr<Square> square) :
	m_square(square),
	m_player(player),
	m_position({}),
	m_state(1),
	m_stateValue(0)
{
	m_selectedWeapon = player->getEquipmentManager()->getDefaultWeapon();
}

void SquarePlayer::close()
{

}

void SquarePlayer::changeWeapon(uint32_t itemId)
{
	m_selectedWeapon = itemId;

	m_square->sendPacket(SendUpdatePlayerEquipment(shared_from_this()));
}

void SquarePlayer::move(std::array<float, 3> position, uint8_t direction, uint8_t moveType)
{
	m_position = position;

	m_square->sendPacket(SendSquarePlayerMove(m_player->getId(), position, moveType, direction));
}

void SquarePlayer::chat(const std::u16string& message)
{
	m_square->sendPacket(SendSquareChatMessage(m_player->getName(), message));
}

void SquarePlayer::emote(const uint32_t emoteId)
{
	m_square->sendPacketExcept(SendUseEmote(m_player->getId(), emoteId), m_player->getId());
}

uint32_t SquarePlayer::getSelectedWeapon()
{
	return m_selectedWeapon;
}

void SquarePlayer::setState(uint32_t state, uint8_t stateValue)
{
	m_state = state;
	m_stateValue = stateValue;

	m_square->sendPacket(SendUpdateSquarePlayerState(shared_from_this(), m_stateValue));
}

uint32_t SquarePlayer::getState()
{
	return m_state;
}

uint8_t SquarePlayer::getStateValue()
{
	return m_stateValue;
}

std::shared_ptr<Square> SquarePlayer::getSquare()
{
	return m_square;
}

std::shared_ptr<Player> SquarePlayer::getPlayer()
{
	return m_player;
}

std::array<float, 3> SquarePlayer::getPosition()
{
	return m_position;
}

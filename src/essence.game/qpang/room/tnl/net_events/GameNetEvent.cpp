#include "GameNetEvent.h"

#include <tnl/tnlNetInterface.h>
#include <tnl/tnlUDP.h>

#include "qpang/room/tnl/GameConnection.h"

GameNetEvent::GameNetEvent()
	: NetEvent{ NetEvent::GuaranteedOrdered, NetEvent::DirUnset }
{
}

GameNetEvent::GameNetEvent(GameNetId gameNetId, NetEvent::GuaranteeType guaranteeType, NetEvent::EventDirection eventDirection) :
	TNL::NetEvent(guaranteeType, eventDirection),
	id(gameNetId)
{
}

std::u16string GameNetEvent::byteBufferToString(ByteBuffer* buffer, uint32_t length)
{
	std::u16string string;

	uint32_t bufferStringLength = buffer->getBufferSize() / 2 - 2;
	uint32_t sizeToCopy = (bufferStringLength > length ? length : bufferStringLength) * 2 + 2;

	string.resize(sizeToCopy / 2 - 2);
	std::memcpy(string.data(), buffer->getBuffer(), sizeToCopy);
	string.erase(std::find(string.begin(), string.end(), '\0'), string.end());

	return string;
}

std::u16string GameNetEvent::readByteBuffer(BitStream* stream, uint32_t length)
{
	ByteBuffer* byteBuffer = new ByteBuffer(length);
	stream->read(byteBuffer);

	uint32_t bufferStringLength = byteBuffer->getBufferSize() / 2 - 2;

	uint32_t sizeToCopy = (bufferStringLength > length ? length : bufferStringLength) * 2 + 2;

	std::u16string string;
	string.resize(sizeToCopy / 2 - 2);

	std::memcpy(string.data(), byteBuffer->getBuffer(), sizeToCopy);

	delete byteBuffer;

	return string;
}

void GameNetEvent::writeByteBuffer(BitStream* stream, const std::u16string& string, uint32_t length)
{
	uint32_t sizeToCopy = (string.size() > length ? length : string.size()) * 2 + 2;

	ByteBuffer* byteBuffer = new ByteBuffer(sizeToCopy);

	std::memcpy(byteBuffer->getBuffer(), string.c_str(), sizeToCopy);

	stream->write(byteBuffer);

	delete byteBuffer;
}

void GameNetEvent::notifyPosted(EventConnection* ps)
{
}

void GameNetEvent::notifyDelivered(EventConnection* ps)
{

}

void GameNetEvent::handle(GameConnection* conn, Player::Ptr player)
{
	std::cout << "[GameNetEvent::handle] Unhandled event: " << id << std::endl;
}
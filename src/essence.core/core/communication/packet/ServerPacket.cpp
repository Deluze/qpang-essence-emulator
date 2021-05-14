#include "ServerPacket.h"

ServerPacket::ServerPacket(uint16_t packetId) :
	Packet(packetId),
	m_cryptoType(BlowfishInstance::CryptoType::AUTHENTICATED)
{
	writeInt(0); // ??
}

void ServerPacket::setCryptoType(BlowfishInstance::CryptoType cryptoType)
{
	m_cryptoType = cryptoType;
}

BlowfishInstance::CryptoType ServerPacket::getCryptoType()
{
	return m_cryptoType;
}

void ServerPacket::writeLong(uint64_t val)
{
	payload.resize(m_currentByte + 8);

	if (payload.size() != m_currentByte + 8)
		throw "ServerPacket::writeLong failed to allocate memory for payload";

	std::memcpy(payload.data() + m_currentByte, &val, 8);

	m_currentByte += 8;
}

void ServerPacket::writeInt(uint32_t val)
{
	payload.resize(m_currentByte + 4);

	if (payload.size() != m_currentByte + 4)
		throw "ServerPacket::writeInt failed to allocate memory for payload";

	std::memcpy(payload.data() + m_currentByte, &val, 4);

	m_currentByte += 4;
}

void ServerPacket::writeShort(uint16_t val)
{
	payload.resize(m_currentByte + 2);

	if (payload.size() != m_currentByte + 2)
		throw "ServerPacket::writeShort failed to allocate memory for payload";

	std::memcpy(payload.data() + m_currentByte, &val, 2);

	m_currentByte += 2;
}

void ServerPacket::writeByte(uint8_t val)
{
	payload.resize(m_currentByte + 1);

	if (payload.size() != m_currentByte + 1)
		throw "ServerPacket::writeByte failed to allocate memory for payload";

	std::memcpy(payload.data() + m_currentByte, &val, 1);

	m_currentByte += 1;
}

void ServerPacket::writeFlag(bool val)
{
	writeByte(val);
}

void ServerPacket::writeString(const std::u16string& val, size_t n)
{
	size_t copySize = n * 2 + 2;

	payload.resize(m_currentByte + copySize);

	if (payload.size() != m_currentByte + copySize)
		throw "ServerPacket::writeString failed to allocate memory for payload";


	std::memcpy(payload.data() + m_currentByte, val.data(), val.size() > n ? copySize : val.size() * 2 + 2);

	m_currentByte += copySize;
}

void ServerPacket::writeChecksum()
{
	static std::array<uint8_t, 8> checksumBytes = { 0x9C, 0x14, 0xED, 0x29, 0xF2, 0xB5, 0x83, 0x7A };

	uint16_t extra = (payload.size() + 2) & 7;
	if (!extra)
	{
		return;
	}

	uint16_t amount = 8 - extra;
	//buffer.resize(buffer.size() + amount + 2);
	for (uint16_t i{ 0 }; i < amount; ++i)
	{
		payload.push_back(payload[i] ^ checksumBytes[i]);
	}

	payload.push_back(static_cast<uint8_t>(amount & 0xFF));
	payload.push_back(static_cast<uint8_t>((amount >> 8) & 0xFF));
}

void ServerPacket::writeEmpty(uint32_t pad, uint8_t val)
{
	m_currentByte += pad;

	payload.resize(m_currentByte, val);
}

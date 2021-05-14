#pragma once

#include <string>
#include <array>

#include "core/communication/packet/Packet.h"
#include "core/communication/encryption/BlowfishInstance.h"

class ServerPacket : public Packet
{
public:
	ServerPacket(uint16_t packetId);

	void setCryptoType(BlowfishInstance::CryptoType cryptoType);
	BlowfishInstance::CryptoType getCryptoType();

	void writeLong(uint64_t val);
	void writeInt(uint32_t val);
	void writeShort(uint16_t val);
	void writeByte(uint8_t val);
	void writeFlag(bool val);
	void writeString(const std::u16string& val, size_t n);
	void writeChecksum();
	void writeEmpty(uint32_t pad, uint8_t val = 0);

	template<typename T, size_t N>
	void writeArray(std::array<T, N> arr)
	{
		const size_t argSize = sizeof(T) * N;

		payload.resize(m_currentByte + argSize);

		std::memcpy(payload.data() + m_currentByte, arr.data(), argSize);

		m_currentByte += argSize;
	};

private:
	BlowfishInstance::CryptoType m_cryptoType;
};
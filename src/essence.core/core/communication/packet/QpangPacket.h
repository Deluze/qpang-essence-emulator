#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>

#include "core/communication/packet/Packet.h"

class QpangPacket : public Packet
{

public:
	QpangPacket();

	void readEmpty(uint16_t pad);
	uint64_t readLong();
	uint32_t readInt();
	uint16_t readShort();
	uint8_t readByte();
	std::u16string readString(size_t n);
	std::u16string readStringTerminated(size_t n);
	bool readFlag();

	template<typename T, size_t N>
	std::array<T, N> readArray()
	{
		const size_t sizeToRead = sizeof(T) * N;
		std::array<T, N> arr;

		std::memcpy(arr.data(), payload.data() + m_currentByte, sizeToRead);

		m_currentByte += sizeToRead;
		
		return arr;
	}
};
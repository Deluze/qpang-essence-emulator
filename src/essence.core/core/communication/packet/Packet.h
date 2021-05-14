#pragma once

#include <vector>

/// Packet Structure:
///
/// Packet::Header (0)
/// Packet::PayloadHeader (4)
/// Packet::Payload (8)

class Packet
{
public:
	struct Header
	{
		uint16_t length;
		bool isEncrypted;
		bool unk;
	};

	struct PayloadHeader
	{
		uint16_t payloadLength; // does not include the payload header
		uint16_t packetId;
	};

	Packet()
	{
		m_currentByte = sizeof(Packet::PayloadHeader);
	}

	Packet(uint16_t packetId)
	{
		m_packetId = packetId;
		m_currentByte = sizeof(Packet::PayloadHeader);

		payload.resize(m_currentByte);
	}

	void writePayloadHeader()
	{
		// packet id should've already been written by now.
		const size_t payloadLength = payload.size() + sizeof(Packet::PayloadHeader);
		std::memcpy(payload.data(), &payloadLength, 2);
		std::memcpy(payload.data() + 2, &m_packetId, 2);
	}

	uint16_t getPacketLength()
	{
		return header.length;
	}

	uint16_t getPacketId()
	{
		return *reinterpret_cast<uint16_t*>(payload.data() + 2);
	}

	bool isEncrypted()
	{
		return !!header.isEncrypted;
	}

	template<typename T>
	void write(T arg)
	{
		const size_t argSize = sizeof(arg);

		payload.resize(m_currentByte + argSize);

		if (payload.size() != m_currentByte + argSize)
			throw "ServerPacket::write<T> failed to allocate memory for payload";

		memcpy(payload.data() + m_currentByte, &arg, argSize);

		m_currentByte += argSize;
	}

	template<typename T>
	void write(T arg, uint16_t pos)
	{
		memcpy(payload.data() + pos, &arg, sizeof(arg));
	}

	template<typename T>
	T read()
	{
		const size_t argSize = sizeof(T);

		if (payload.size() < argSize + m_currentByte)
			throw std::exception("Packet::read invalid packet length");

		T val = *reinterpret_cast<T*>(payload.data() + m_currentByte);

		m_currentByte += argSize;

		return val;
	}

	Packet::Header header{};
	std::vector<char> payload{};
protected:
	uint16_t m_currentByte{ 0 };
	uint16_t m_packetId;
};
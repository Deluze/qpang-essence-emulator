#include "QpangPacket.h"

QpangPacket::QpangPacket() : Packet()
{
	m_currentByte = 8;
}

void QpangPacket::readEmpty(uint16_t pad)
{
	if (m_currentByte + pad > payload.size())
		throw std::exception("QpangPacket::readEmpty too much padding");

	m_currentByte += pad;
}

uint64_t QpangPacket::readLong()
{
	return read<uint64_t>();
}

uint32_t QpangPacket::readInt()
{
	return read<uint32_t>();
}

uint16_t QpangPacket::readShort()
{
	return read<uint16_t>();
}

uint8_t QpangPacket::readByte()
{
	return read<uint8_t>();
}

std::u16string QpangPacket::readString(size_t n)
{
	if (payload.size() < n * 2 + 2)
		throw std::exception("QpangPacket::readString invalid string length");

	std::u16string string = std::u16string(reinterpret_cast<char16_t*>(payload.data() + m_currentByte), n);

	m_currentByte += n * 2 + 2;

	return string;
}

std::u16string QpangPacket::readStringTerminated(size_t n)
{
	const auto string = readString(n);

	return string.substr(0, string.find(u'\0'));
}

bool QpangPacket::readFlag()
{
	return !!readByte();
}

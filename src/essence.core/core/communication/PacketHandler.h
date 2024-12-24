#pragma once

#include <memory>
#include <unordered_map>

#include "core/communication/packet/QpangPacket.h"

class PacketEvent;
class QpangConnection;

class PacketHandler  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	virtual ~PacketHandler() = default;
	virtual void handle(std::shared_ptr<QpangConnection> conn, QpangPacket pack);
protected:
	void add(uint16_t packetId, PacketEvent* packEvent);
	std::unordered_map<uint16_t, PacketEvent*> m_events;
};
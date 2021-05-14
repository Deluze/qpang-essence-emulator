#pragma once

#include <unordered_map>
#include <memory>

#include "core/communication/packet/QpangPacket.h"

class PacketEvent;
class QpangConnection;

class PacketHandler
{
public:
	virtual void handle(std::shared_ptr<QpangConnection> conn, QpangPacket pack);
protected:
	void add(uint16_t packetId, PacketEvent* packEvent);
	std::unordered_map<uint16_t, PacketEvent*> m_events;
};
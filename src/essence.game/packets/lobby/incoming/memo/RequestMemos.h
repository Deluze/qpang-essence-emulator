#pragma once

#include "core/communication/packet/PacketEvent.h"

#include <vector>

#include "qpang/player/Player.h"
#include "qpang/player/memo/MemoManager.h"

#include "packets/lobby/outgoing/memo/Memos.h"

class RequestMemos : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		if (const auto player = conn->getPlayer(); player != nullptr)
		{
			const std::vector<Memo> memos = player->getMemoManager()->list();
			conn->send(Memos(memos));
		}
	}
};
#pragma once

#include <vector>

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/memo/SendMemoList.h"
#include "qpang/player/Player.h"
#include "qpang/player/memo/MemoManager.h"

class RequestMemos final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		if (const auto player = conn->getPlayer(); player != nullptr)
		{
			const std::vector<Memo> memos = player->getMemoManager()->list();
			conn->send(SendMemoList(memos));
		}
	}
};

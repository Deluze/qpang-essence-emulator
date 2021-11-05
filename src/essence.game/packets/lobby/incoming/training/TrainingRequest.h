#pragma once

#include "core/communication/packet/PacketEvent.h"

class TrainingRequest final : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		//pack.readEmpty(1);
		////const auto unk01 = pack.readLong();
		//const auto unk01 = pack.readShort();
		////const auto unk02 = pack.readInt();
		//const auto unk02 = pack.readShort();
		//const auto unk03 = pack.readShort();

		///*const auto unk01 = pack.readShort();
		//const auto unk03 = pack.readShort();
		//const auto unk04 = pack.readShort();*/

		//pack.readEmpty(1);

		//std::cout << "TrainingRequest::handle >> unk01: " << unk01 << ", unk02:" << unk02 << ", unk03:" << unk03 << std::endl;
	}
};
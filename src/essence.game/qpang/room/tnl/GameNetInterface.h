#pragma once

#include <iostream>

#include "qpang/room/tnl/GameConnection.h"

#include <tnl/tnl.h>
#include <tnl/tnlNetInterface.h>
#include <tnl/tnlNetConnection.h>
#include <tnl/tnlBitStream.h>
#include <tnl/tnlClientPuzzle.h>
#include <tnl/tnlCertificate.h>
#include <libtomcrypt/mycrypt.h>

class GameNetInterface : public TNL::NetInterface
{
public:
	GameNetInterface(const TNL::Address& addr);
	~GameNetInterface() = default;
	void processPacket(const TNL::Address& address, TNL::BitStream* packetStream) override;
protected:
	void handleInfoPacket(const TNL::Address& address, TNL::U8 packetType, TNL::BitStream* stream) override;
};
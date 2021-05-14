#include "AuthPacketHandler.h"

#include "packets/incoming/Handshake.h"
#include "packets/incoming/LoginRequest.h"

AuthPacketHandler::AuthPacketHandler()
{
	add(1, new Handshake());
	add(500, new LoginRequest());
}

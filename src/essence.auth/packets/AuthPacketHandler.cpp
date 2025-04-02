#include "AuthPacketHandler.h"

#include "packets/incoming/HandleHandshakeRequest.h"
#include "packets/incoming/LoginRequest.h"

AuthPacketHandler::AuthPacketHandler()
{
	add(1, new HandleHandshakeRequest());
	add(500, new LoginRequest());
}

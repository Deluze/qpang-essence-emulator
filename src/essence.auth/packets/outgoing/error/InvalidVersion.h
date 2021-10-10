#pragma once

#include "core/communication/packet/ServerErrorPacket.h"

class InvalidVersion : public ServerErrorPacket
{
public:
	/***
	* RsLoginFail - 0 - FR_NOERROR
	* RsLoginFail - 1 - FR_UNKNOWN
	* RsLoginFail - 2 - FR_INVALID_SEQUENCE
	* RsLoginFail - 3 - FR_CHECKSUM_FAILED
	* RsLoginFail - 4 - FR_DECRYPT_FAILED
	* RsLoginFail - 5 - FR_NOT_CERTIFIED
	*
	* RsLoginFail - 100 - Login failed due to server error
	* RsLoginFail - 101 - Login failed due to server error
	* RsLoginFail - 102 - Username is invalid or does not exist
	* RsLoginFail - 103 - FR_DB_DATA_EXISTS
	* 
	* RsLoginFail - 200 - Login failed due to server error
	* RsLoginFail - 201 - Login failed due to server error
	* RsLoginFail - 202 - Login failed due to server error
	* RsLoginFail - 203 - Username is invalid or does not exist
	* RsLoginFail - 204 - FR_AS_CERTIFY_FAILED
	* 
	* RsLoginFail - 300 - FR_CS_INTERNAL
	* RsLoginFail - 301 - FR_CS_CONN_FAILED
	* RsLoginFail - 302 - FR_CS_INVALID_GAME
	*
	*
	* RsLoginFail - 1100 - Server Error: Unknown
	* RsLoginFail - 1101 - Wrong password
	* RsLoginFail - 1102 - Account does not exist
	* RsLoginFail - 1103 - Banned region / Banned IP area
	* RsLoginFail - 1104 - Banned region / Banned IP area
	* RsLoginFail - 1105 - Username contains banned words
	* RsLoginFail - 1106 - Login failed due to server error
	* RsLoginFail - 1107 - Login failed due to server error
	* RsLoginFail - 1108 - Application was not submitted
	***/
	InvalidVersion() : ServerErrorPacket(502)
	{
		writeInt(503);
	}
};
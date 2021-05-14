#pragma once

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "core/Emulator.h"
#include "core/database/Database.h"
#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"

#include "packets/lobby/outgoing/account/VerificationFailure.h"
#include "packets/lobby/outgoing/account/Banned.h"
#include "packets/lobby/outgoing/account/Authenticated.h"

class Login : public PacketEvent
{
public:
	bool isUnauthorizedPacket() override
	{
		return true;
	}

	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		std::array<uint8_t, 16> uuidArr = packet.read<std::array<uint8_t, 16>>();

		boost::uuids::uuid uuid;
		std::memcpy(uuid.data, &uuidArr, boost::uuids::uuid::static_size());
		std::string uuidStr = boost::uuids::to_string(uuid);

		uint32_t userId = 0;

		{
			Statement::Ptr stmt = DATABASE->prepare("SELECT id FROM users WHERE session_uuid = ?");
			stmt->bindString(uuidStr.c_str());
			StatementResult::Ptr res = stmt->fetch();

			if (!res->hasResults()) // invalid uuid
			{
				conn->send(VerificationFailure());
				return conn->close();
			}

			userId = res->getInt("id");
		}

		bool isBanned = Game::instance()->getBanManager()->isBanned(userId);

		if (isBanned)
		{
			conn->send(Banned());
			return conn->close();
		}

		uint32_t playerId = 0;

		{
			auto stmt = DATABASE->prepare("SELECT id FROM players WHERE user_id = ?");
			stmt->bindInteger(userId);
			auto res = stmt->fetch();

			if (!res->hasResults())
				return conn->close();

			playerId = res->getInt("id");
		}

		Game::instance()->createPlayer(conn, playerId);

		conn->send(Authenticated(conn->getPlayer()));
	}
};

#pragma once

#include <boost/uuid/uuid_io.hpp>

#include "core/Emulator.h"
#include "core/database/Database.h"
#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"

#include "packets/lobby/outgoing/account/SendAccountVerificationFailure.h"
#include "packets/lobby/outgoing/account/SendAccountBanned.h"
#include "packets/lobby/outgoing/account/SendAccountAuthenticated.h"

class Login final : public PacketEvent
{
public:
	bool isUnauthorizedPacket() override
	{
		return true;
	}

	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto uuidArr = packet.read<std::array<uint8_t, 16>>();

		boost::uuids::uuid uuid{};
		std::memcpy(uuid.data, &uuidArr, boost::uuids::uuid::static_size());

		const std::string uuidStr = to_string(uuid);

		uint32_t userId;

		{
			const Statement::Ptr statement = DATABASE->prepare("SELECT id FROM users WHERE session_uuid = ?");

			statement->bindString(uuidStr.c_str());
			const StatementResult::Ptr result = statement->fetch();

			if (!result->hasResults()) // invalid uuid
			{
				conn->send(SendAccountVerificationFailure());

				return conn->close();
			}

			userId = result->getInt("id");
		}

		if (Game::instance()->getBanManager()->isBanned(userId))
		{
			conn->send(SendAccountBanned());

			return conn->close();
		}

		uint32_t playerId;

		{
			const auto statement = DATABASE->prepare("SELECT id FROM players WHERE user_id = ?");

			statement->bindInteger(userId);

			const auto result = statement->fetch();

			if (!result->hasResults())
			{
				return conn->close();
			}

			playerId = result->getInt("id");
		}

		Game::instance()->createPlayer(conn, playerId);

		conn->send(SendAccountAuthenticated(conn->getPlayer()));
	}
};

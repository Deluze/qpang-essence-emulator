#pragma once

#include <string>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "core/communication/packet/PacketEvent.h"
#include "core/Emulator.h"
#include "core/database/Database.h"
#include "core/config/ConfigManager.h"

#include "Authenticator.h"

#include "packets/outgoing/SendAccountLoginSuccess.h"
#include "packets/outgoing/error/SendInvalidGameVersion.h"
#include "packets/outgoing/error/SendInvalidAccountUsername.h"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

// TODO: Refactor this class.
class LoginRequest : public PacketEvent
{
public:
	bool isUnauthorizedPacket() override
	{
		return true;
	}

	void storeIp(uint32_t userId, const std::string& ip)
	{
		auto prep = DATABASE->prepare("INSERT INTO user_ips (user_id, ip) VALUES (?, ?)");
		prep->bindInteger(userId);
		prep->bindString(ip.c_str());
		prep->execute();
	}

	void invalidUsername(QpangConnection::Ptr conn)
	{
		conn->send(SendInvalidAccountUsername());
		conn->close();
	}

	void invalidVersion(QpangConnection::Ptr conn)
	{
		conn->send(SendInvalidGameVersion());
		conn->close();
	}
	
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{		
		packet.readEmpty(16);
		std::u16string username = packet.readString(20);
		std::u16string password = packet.readString(16);
		packet.readEmpty(12);
		uint32_t revision = packet.readInt();

		std::string revisionStr = std::to_string(revision);
		std::string revisionConf = CONFIG_MANAGER->getString("GAME_REVISION");

		if (revisionStr != revisionConf)
		{
			invalidVersion(conn);
			return;
		}

		const tcp::socket& socket = conn->getSocket();
		const auto ipAddress = socket.remote_endpoint().address().to_string();
		auto failInfo = m_failedLoginAttempts[ipAddress];

		uint32_t loginAttemptCooldown = CONFIG_MANAGER->getInt("LOGIN_ATTEMPT_COOLDOWN");
		uint32_t maxFailedLogins = CONFIG_MANAGER->getInt("MAX_FAILED_LOGIN_ATTEMPTS");

		if (failInfo.second != NULL && time(NULL) >= failInfo.second + loginAttemptCooldown)
		{
			m_failedLoginAttempts.erase(ipAddress);
			failInfo = m_failedLoginAttempts[ipAddress];
		}
		else
		{
			if (failInfo.first >= maxFailedLogins)
			{
				invalidUsername(conn);
				return;
			}
		}

		if (!Authenticator::verify(username, password))
		{
			const auto attempts = failInfo.first + 1;
			auto timestamp = failInfo.second == NULL ? time(NULL) : failInfo.second;

			if (attempts == maxFailedLogins)
			{
				std::cout << "Timed out " << ipAddress << " - Too many failed login attempts.\n";
				timestamp = time(NULL);
			}

			m_failedLoginAttempts[ipAddress] = std::make_pair(attempts, timestamp);

			invalidUsername(conn);
			return;
		} // ban check happens on lobby server

		auto needsWhitelist = CONFIG_MANAGER->getString("GAME_WHITELIST") == "1";

		auto stmt = DATABASE->prepare("SELECT * FROM users WHERE name = ?");
		stmt->bindString(username.c_str());
		auto res = stmt->fetch();

		if (!res->hasResults())
		{
			invalidVersion(conn);
			return;
		}

		if (needsWhitelist)
		{
			auto isWhiteListed = res->getFlag("whitelisted");

			if (!isWhiteListed)
			{
				invalidVersion(conn);
				return;
			}
		}

		const auto userId = res->getInt("id");
		const auto ipAddressDb = res->getUTF8String("registration_ip");
		const bool isRegistrationIpFilled = !ipAddressDb.empty();
		
		// register ip is null...
		if (!isRegistrationIpFilled)
		{
			auto prep = DATABASE->prepare("UPDATE users SET registration_ip = ? WHERE id = ?");
			prep->bindString(ipAddress.c_str());
			prep->bindInteger(userId);
			prep->execute();
		}

		// CHECK IF IP IS ALREADY IN USER_IPS table
		auto ipStmt = DATABASE->prepare("SELECT ip FROM user_ips WHERE ip = ? AND user_id = ?");
		ipStmt->bindString(ipAddress.c_str());
		ipStmt->bindInteger(userId);
		auto result = ipStmt->fetch();

		// IP not found for this user INSERT it. Because it's a unique ip for current user.
		if (!result->hasResults())
		{
			storeIp(userId, ipAddress);
		}
		
		// If registration ip is not the same as current
		if (isRegistrationIpFilled && ipAddressDb != ipAddress && CONFIG_MANAGER->getInt("ACCOUNT_IP_LOCK") == 1)
		{
			invalidUsername(conn);
			return;
		}
		
		boost::uuids::uuid uuid = m_uuidGenerator();
		std::array<uint8_t, 16> uuidArr;
		std::memcpy(uuidArr.data(), uuid.data, 16);

		auto updateSessionStmt = DATABASE->prepare("UPDATE users SET session_uuid = ? WHERE name = ?");

		updateSessionStmt->bindString(boost::uuids::to_string(uuid).c_str());
		updateSessionStmt->bindString(username.c_str());
		updateSessionStmt->execute();

		conn->send(SendAccountLoginSuccess(uuidArr, inet_addr(CONFIG_MANAGER->getString("HOST").c_str())));
	}
private:
	boost::uuids::random_generator m_uuidGenerator;
	std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> m_failedLoginAttempts;
};
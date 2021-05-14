#include "Authenticator.h"

#include <functional>

#include "crypt/bcrypt.h"

#include "core/Emulator.h"
#include "core/database/Database.h"
#include "utils/StringConverter.h"

Authenticator::Authenticator()
{
	m_server = new QpangServer(8003);
	m_packHandler = new AuthPacketHandler();

	m_server->setAcceptHandler(std::bind(&Authenticator::onNewConnection, this, std::placeholders::_1));

	Emulator::instance()->attachServer(m_server);

	Statement::Ptr stmt = DATABASE->prepare("UPDATE users SET session_uuid = ?");
	stmt->bindString("");
	stmt->execute();
}

void Authenticator::onNewConnection(QpangConnection::Ptr conn)
{
	conn->setPacketHandler(m_packHandler);
	conn->setDisconnectHandler(std::bind(&Authenticator::onLobbyConnectionClosed, this, std::placeholders::_1));
}

void Authenticator::onLobbyConnectionClosed(QpangConnection::Ptr conn)
{
	m_server->removeConnection(conn);
}

void Authenticator::onConnectionData(QpangConnection::Ptr conn, QpangPacket packet)
{
	m_packHandler->handle(conn, packet);
}

bool Authenticator::verify(std::u16string username, std::u16string password)
{
	if (username.empty() || password.empty())
		return false;

	Statement::Ptr stmt = DATABASE->prepare("SELECT password FROM users WHERE name = ?");

	stmt->bindString(username);

	StatementResult::Ptr res = stmt->fetch();

	if (!res->hasResults())
		return false;

	std::string pass = StringConverter::Utf16ToUtf8(password);
	std::string hash = StringConverter::Utf16ToUtf8(res->getString("password", 60));

	return bcrypt_checkpw(pass.c_str(), hash.c_str()) == 0;
}

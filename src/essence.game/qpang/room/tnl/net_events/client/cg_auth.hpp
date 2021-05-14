#ifndef CG_AUTH_HPP
#define CG_AUTH_HPP

#include <tnl/tnlEventConnection.h>

#include "GameNetEvent.h"

#include "qpang/Game.h"
#include "qpang/room/RoomServer.h"

class CGAuth : public GameNetEvent
{
	typedef NetEvent Parent;

public:
	enum CMD : U32 {
		REQUEST = 0,
		AUTHENTICATED = 1,
		FAIL = 2,
	};

	CGAuth() : GameNetEvent{ CG_AUTH, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};
	CGAuth(U32 playerId, U32 cmd) : GameNetEvent{ CG_AUTH, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } { this->playerId = playerId; this->cmd = cmd; this->unk_03 = AUTHENTICATED; };
	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(playerId);
		bstream->write(cmd);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(port);
		bstream->write(ip);
		bstream->write(unk_07);
		bstream->write(unk_08);
		bstream->write(unk_09);
		bstream->write(unk_10);
	};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&cmd);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
		bstream->read(&port);
		bstream->read(&ip);
		bstream->read(&unk_07);
		bstream->read(&unk_08);
		bstream->read(&unk_09);
		bstream->read(&unk_10);
	};

	void process(EventConnection* ps)
	{
		post<CGAuth>(ps);
	};

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		if (cmd != CMD::REQUEST)
			return;

		if (Game::instance()->getRoomServer()->createConnection(playerId, gameConnection))
			conn->postNetEvent(new CGAuth(playerId, CMD::AUTHENTICATED));
		else
			conn->postNetEvent(new CGAuth(playerId, CMD::FAIL));
	}

	TNL_DECLARE_CLASS(CGAuth);

	U32 playerId;
	U32 cmd;
	U8 unk_03 = 3;
	U32 unk_04;
	U16 port;
	U32 ip;
	//Sent from authserver
	U32 unk_07;
	U32 unk_08;
	U32 unk_09;
	U32 unk_10;
};

#endif
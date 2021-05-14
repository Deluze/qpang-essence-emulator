#ifndef GC_MESG_HPP
#define GC_MESG_HPP

#include "GameNetEvent.h"

#include <cstdint>
#include <string>

class GCMesg : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCMesg() : GameNetEvent{ GC_MESG, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCMesg(uint32_t playerId, uint32_t cmd, std::u16string& nickname, std::u16string& message) : GameNetEvent{ GC_MESG, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny }
	{
		this->playerId = playerId;
		this->cmd = cmd;
		this->nickname = nickname;
		this->message = message;
	};

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(playerId);
		bstream->write(cmd);
		writeByteBuffer(bstream, nickname, 16);
		writeByteBuffer(bstream, message, message.size() % 254);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 playerId;
	U32 cmd;

	std::u16string nickname;
	std::u16string message;

	TNL_DECLARE_CLASS(GCMesg);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
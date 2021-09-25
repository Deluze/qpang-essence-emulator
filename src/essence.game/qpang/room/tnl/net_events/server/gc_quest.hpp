#ifndef GC_QUEST_HPP
#define GC_QUEST_HPP

#include "GameNetEvent.h"

class GCQuest : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCQuest() : GameNetEvent{ GC_QUEST, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};
	GCQuest(uint32_t playerId, uint8_t cmd) : GameNetEvent{ GC_QUEST, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {
		this->playerId = playerId;
		this->cmd = cmd;
	}

	GCQuest(uint32_t playerId, uint8_t cmd, uint16_t unk02) : GameNetEvent{ GC_QUEST, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {
		this->playerId = playerId;
		this->cmd = cmd;
		this->unk_02 = unk02;
	}

	GCQuest(uint32_t playerId, uint8_t cmd, uint16_t unk02, uint8_t unk03) : GameNetEvent{ GC_QUEST, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {
		this->playerId = playerId;
		this->cmd = cmd;
		this->unk_02 = unk02;
		this->unk_03 = unk03;
	};

	enum CMD : uint8_t
	{
		SHOW_QUEST = 1,
		COMPLETE_QUEST = 3
	};

	U32 playerId;
	U8 cmd;
	U16 unk_02;
	U8 unk_03;

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(playerId);
		bstream->write(cmd);
		bstream->write(unk_02);
		bstream->write(unk_03); // amount of tasks done for quest
	};

	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCQuest);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
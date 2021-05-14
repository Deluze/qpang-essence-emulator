#ifndef GC_GAME_ITEM_HPP
#define GC_GAME_ITEM_HPP

#include <vector>
#include "GameNetEvent.h"

class GCGameItem : public GameNetEvent
{
	typedef NetEvent Parent;

public:

	struct Item 
	{
		U32 itemId;
		U32 itemUid;
		F32 x, y, z = 0xFF;
		U32 skillId = 0;
	};

	GCGameItem() : GameNetEvent{ GC_GAME_ITEM, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient} {}

	GCGameItem(U8 cmd, std::vector<GCGameItem::Item> items, U32 weaponIdentifier = 0x00) : GameNetEvent{ GC_GAME_ITEM, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->cmd = cmd;
		this->items = items;
		this->weaponId = weaponIdentifier;
	}
	
	GCGameItem(U8 cmd, U32 playerId, U32 itemId, U32 uid, uint32_t skillId = 0x00) : GameNetEvent{ GC_GAME_ITEM, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirServerToClient }
	{
		this->cmd = cmd;
		this->playerId = playerId;
		this->itemId = itemId;
		this->uid = uid;
		this->skillId = skillId;
	}

	void pack(EventConnection* conn, BitStream* bstream)
	{
		bstream->write(playerId);
		bstream->write(cmd);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(itemId);
		bstream->write(uid);
		bstream->write(skillId);
		bstream->write(unk_08);
		bstream->write(U16(items.size()));
		
		for (auto &item : items)
		{
			bstream->write(U32(0));
			bstream->write(item.itemId);
			bstream->write(item.itemUid);
			bstream->write(item.skillId);
			bstream->write(U32(0));
			bstream->write(item.x);
			bstream->write(item.y);
			bstream->write(item.z);
			bstream->write(weaponId);
		}
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	U32 playerId = 0;
	U8 cmd = 6; // cmd [1 = pick up, 6 = spawn, 14 = refill ammo, others unknown]
	U32 unk_03 = 0;
	U32 unk_04 = 0;
	U32 itemId = 0; // pickup only
	U32 uid = 0; // pickup only
	U32 skillId = 0;
	U32 unk_08 = 0;
	U16 count = 0;

	/// stuff I don't want to clutter the game item class with
	U32 weaponId = 0; // refill ammo only

	std::vector<GCGameItem::Item> items;
	
	TNL_DECLARE_CLASS(GCGameItem);
};


#endif // !GC_ARRANGED_ACCEPT_HPP
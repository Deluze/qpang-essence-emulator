#ifndef CG_ARRANGED_COMPLETE_HPP
#define CG_ARRANGED_COMPLETE_HPP

#include "GameNetEvent.h"

class CGArrangedComplete : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 playerUid; //88
	U32 targetUid; //92


	CGArrangedComplete() : GameNetEvent{ CG_ARRANGED_COMPLETE, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirClientToServer } {};
	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->read(&playerUid);
		bstream->read(&targetUid);
	};
	void process(EventConnection* ps) 
	{
	};



	TNL_DECLARE_CLASS(CGArrangedComplete);
};

#endif
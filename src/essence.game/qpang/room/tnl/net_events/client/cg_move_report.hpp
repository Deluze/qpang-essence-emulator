#ifndef CG_MOVE_REPORT_H
#define CG_MOVE_REPORT_H

#include "GameNetEvent.h"
class CGMoveReport : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	U32 playerId; //88
	F32 xPosition; //92
	F32 yPosition; //96
	F32 zPosition; //100

	// pitch & yaw?
	F32 unk_05; //104
	F32 unk_06; //108

	CGMoveReport() : GameNetEvent{ CG_MOVE_REPORT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&playerId);
		bstream->read(&xPosition);
		bstream->read(&yPosition);
		bstream->read(&zPosition);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
	};
	void process(EventConnection* ps)
	{
	};

	TNL_DECLARE_CLASS(CGMoveReport);
};
#endif
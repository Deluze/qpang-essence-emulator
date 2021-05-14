#ifndef CC_USER_INFO_H
#define CC_USER_INFO_H

#include "qpang/room/tnl/net_events/GameNetEvent.h"

class CCUserInfo : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CCUserInfo() : GameNetEvent{ CC_USERINFO, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirClientToServer } {};

	U32 unk_01 = 0;
	U32 unk_02;
	U16 unk_03;
	U8 unk_04;
	U16 unk_05;
	U8 unk_06;
	U32 unk_07;
	U32 unk_08;
	U32 unk_09;
	U32 unk_10;
	U32 unk_11;
	U32 unk_12;
	U32 unk_13;
	U32 unk_14;
	U8 unk_15;
	U32 unk_16;
	U8 unk_17;

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream)
	{
		bstream->read(&unk_01);
		bstream->read(&unk_02);
		bstream->read(&unk_03);
		bstream->read(&unk_04);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
		bstream->read(&unk_07);
		bstream->read(&unk_08);
		bstream->read(&unk_09);
		bstream->read(&unk_10);
		bstream->read(&unk_11);
		bstream->read(&unk_12);
		bstream->read(&unk_13);
		bstream->read(&unk_14);
		bstream->read(&unk_15);
		bstream->read(&unk_16);
		bstream->read(&unk_17);
	};

	void process(EventConnection* conn)
	{

	}

	TNL_DECLARE_CLASS(CCUserInfo);
};

#endif
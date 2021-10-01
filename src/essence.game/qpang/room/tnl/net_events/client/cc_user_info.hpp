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

		// Does not trigger?
		//std::cout << "CCUserInfo::unpack >> unk_01: " << (&unk_01) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_02: " << (&unk_02) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_03: " << (&unk_03) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_04: " << (&unk_04) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_05: " << (&unk_05) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_06: " << (&unk_06) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_07: " << (&unk_07) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_08: " << (&unk_08) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_09: " << (&unk_09) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_10: " << (&unk_10) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_11: " << (&unk_11) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_12: " << (&unk_12) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_13: " << (&unk_13) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_14: " << (&unk_14) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_15: " << (&unk_15) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_16: " << (&unk_16) << std::endl;
		//std::cout << "CCUserInfo::unpack >> unk_17: " << (&unk_17) << std::endl;
	};

	void process(EventConnection* conn)
	{

	}

	TNL_DECLARE_CLASS(CCUserInfo);
};

#endif
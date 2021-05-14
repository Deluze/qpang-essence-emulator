#ifndef GC_MASTER_LOG_HPP
#define GC_MASTER_LOG_HPP

#include "GameNetEvent.h"

class GCMasterLog : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCMasterLog() : GameNetEvent{ GC_MASTERLOG, NetEvent::GuaranteeType::Guaranteed, NetEvent::DirAny } {};

	U32 unk_01;
	U32 unk_02;
	U32 unk_03;
	U16 unk_04;
	U16 unk_05;
	U8 unk_06;
	U32 unk_07;
	U32 unk_08;
	U32 unk_09;
	U32 unk_10;
	U32 unk_11;
	U32 unk_12;
	U8 unk_13;
	ByteBuffer* buffer;

	void pack(EventConnection* conn, BitStream* bstream) 
	{
		bstream->write(unk_01);
		bstream->write(unk_02);
		bstream->write(unk_03);
		bstream->write(unk_04);
		bstream->write(unk_05);
		bstream->write(unk_06);
		bstream->write(unk_07);
		bstream->write(unk_08);
		bstream->write(unk_09);
		bstream->write(unk_10);
		bstream->write(unk_11);
		bstream->write(unk_12);
		bstream->write(unk_13);
		bstream->write(&buffer);
	};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};
	TNL_DECLARE_CLASS(GCMasterLog);
};

#endif // !GC_ARRANGED_ACCEPT_HPP
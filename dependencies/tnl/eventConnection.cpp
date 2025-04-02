//-----------------------------------------------------------------------------------
//
//   Torque Network Library
//   Copyright (C) 2004 GarageGames.com, Inc.
//   For more information see http://www.opentnl.org
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   For use in products that are not compatible with the terms of the GNU 
//   General Public License, alternative licensing options are available 
//   from GarageGames.com.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//------------------------------------------------------------------------------------

#include "tnl.h"
#include "tnlEventConnection.h"
#include "tnlBitStream.h"
#include "tnlLog.h"
#include "tnlNetInterface.h"
#include <stdio.h>

namespace TNL {

ClassChunker<EventConnection::EventNote> EventConnection::mEventNoteChunker;

EventConnection::EventConnection()
{
   // event management data:

   mNotifyEventList = NULL;
   mSendEventQueueHead = NULL;
   mSendEventQueueTail = NULL;
   mUnorderedSendEventQueueHead = NULL;
   mUnorderedSendEventQueueTail = NULL;
   mWaitSeqEvents = NULL;

   mNextSendEventSeq = FirstValidSendEventSeq;
   mNextRecvEventSeq = FirstValidSendEventSeq;
   mLastAckedEventSeq = -1;
   mEventClassCount = 0;
   mEventClassBitSize = 0;
}

EventConnection::~EventConnection()
{
   
   while(mNotifyEventList)
   {
      EventNote *temp = mNotifyEventList;
      mNotifyEventList = temp->mNextEvent;
      
      temp->mEvent->notifyDelivered(this, true);
      mEventNoteChunker.free(temp);
   }
   while(mUnorderedSendEventQueueHead)
   {
      EventNote *temp = mUnorderedSendEventQueueHead;
      mUnorderedSendEventQueueHead = temp->mNextEvent;
      
      temp->mEvent->notifyDelivered(this, true);
      mEventNoteChunker.free(temp);
   }
   while(mSendEventQueueHead)
   {
      EventNote *temp = mSendEventQueueHead;
      mSendEventQueueHead = temp->mNextEvent;
      
      temp->mEvent->notifyDelivered(this, true);
      mEventNoteChunker.free(temp);
   }
}

void EventConnection::writeConnectRequest(BitStream *stream)
{
   Parent::writeConnectRequest(stream);
   stream->write(NetClassRep::getNetClassCount(getNetClassGroup(), NetClassTypeEvent));
}

bool EventConnection::readConnectRequest(BitStream *stream, const char **errorString)
{
   if(!Parent::readConnectRequest(stream, errorString))
      return false;

   U32 classCount;
   stream->read(&classCount);

   U32 myCount = NetClassRep::getNetClassCount(getNetClassGroup(), NetClassTypeEvent);
   if(myCount <= classCount)
      mEventClassCount = myCount;
   else
   {
      mEventClassCount = classCount;
      if(!NetClassRep::isVersionBorderCount(getNetClassGroup(), NetClassTypeEvent, mEventClassCount))
         return false;
   }
   mEventClassVersion = NetClassRep::getClass(getNetClassGroup(), NetClassTypeEvent, mEventClassCount-1)->getClassVersion();
   mEventClassBitSize = getNextBinLog2(mEventClassCount);
   return true;
}

void EventConnection::writeConnectAccept(BitStream *stream)
{
   Parent::writeConnectAccept(stream);
   stream->write(mEventClassCount);
}

bool EventConnection::readConnectAccept(BitStream *stream, const char **errorString)
{
   if(!Parent::readConnectAccept(stream, errorString))
      return false;

   stream->read(&mEventClassCount);
   U32 myCount = NetClassRep::getNetClassCount(getNetClassGroup(), NetClassTypeEvent);

   if(mEventClassCount > myCount)
      return false;

   if(!NetClassRep::isVersionBorderCount(getNetClassGroup(), NetClassTypeEvent, mEventClassCount))
      return false;

   mEventClassBitSize = getNextBinLog2(mEventClassCount);
   return true;
}

void EventConnection::processEvent(NetEvent *theEvent)
{
    if (theEvent == nullptr)
    {
        printf("EventConnection::processEvent: theEvent is nullptr!\n");
        return;
    }

   if(getConnectionState() == NetConnection::Connected)
      theEvent->process(this);
}

void EventConnection::packetDropped(PacketNotify *pnotify)
{
   Parent::packetDropped(pnotify);
   EventPacketNotify *notify = static_cast<EventPacketNotify *>(pnotify);

   EventNote *walk = notify->eventList;
   EventNote **insertList = &mSendEventQueueHead;
   EventNote *temp;
   
   while(walk)
   {
      switch(walk->mEvent->mGuaranteeType)
      {
         case NetEvent::GuaranteedOrdered:
            // It was a guaranteed ordered packet, reinsert it back into
            // mSendEventQueueHead in the right place (based on seq numbers)

            TNLLogMessageV(LogEventConnection, ("EventConnection %s: DroppedGuaranteed - %d", getNetAddressString(), walk->mSeqCount));
            while(*insertList && (*insertList)->mSeqCount < walk->mSeqCount)
               insertList = &((*insertList)->mNextEvent);
            
            temp = walk->mNextEvent;
            walk->mNextEvent = *insertList;
            if(!walk->mNextEvent)
               mSendEventQueueTail = walk;
            *insertList = walk;
            insertList = &(walk->mNextEvent);
            walk = temp;
            break;
         case NetEvent::Guaranteed:
            // It was a guaranteed packet, put it at the top of
            // mUnorderedSendEventQueueHead.
            temp = walk->mNextEvent;
            walk->mNextEvent = mUnorderedSendEventQueueHead;
            mUnorderedSendEventQueueHead = walk;
            if(!walk->mNextEvent)
               mUnorderedSendEventQueueTail = walk;
            walk = temp;
            break;
         case NetEvent::Unguaranteed:
            // Or else it was an unguaranteed packet, notify that
            // it was _not_ delivered and blast it.
            walk->mEvent->notifyDelivered(this, false);
            temp = walk->mNextEvent;
            mEventNoteChunker.free(walk);
            walk = temp;
      }
   }
}

void EventConnection::packetReceived(PacketNotify *pnotify)
{
   Parent::packetReceived(pnotify);

   EventPacketNotify *notify = static_cast<EventPacketNotify *>(pnotify);

   EventNote *walk = notify->eventList;
   EventNote **noteList = &mNotifyEventList;

   while(walk)
   {
      EventNote *next = walk->mNextEvent;
      if(walk->mEvent->mGuaranteeType != NetEvent::GuaranteedOrdered)
      {
         walk->mEvent->notifyDelivered(this, true);
         mEventNoteChunker.free(walk);
         walk = next;
      }
      else
      {
         while(*noteList && (*noteList)->mSeqCount < walk->mSeqCount)
            noteList = &((*noteList)->mNextEvent);
         
         walk->mNextEvent = *noteList;
         *noteList = walk;
         noteList = &walk->mNextEvent;
         walk = next;
      }
   }
   while(mNotifyEventList && mNotifyEventList->mSeqCount == mLastAckedEventSeq + 1)
   {
      mLastAckedEventSeq++;
      EventNote *next = mNotifyEventList->mNextEvent;
      TNLLogMessageV(LogEventConnection, ("EventConnection %s: NotifyDelivered - %d", getNetAddressString(), mNotifyEventList->mSeqCount));
      mNotifyEventList->mEvent->notifyDelivered(this, true);
      mEventNoteChunker.free(mNotifyEventList);
      mNotifyEventList = next;
   }
}

void EventConnection::writePacket(BitStream *bstream, PacketNotify *pnotify)
{
   Parent::writePacket(bstream, pnotify);
   EventPacketNotify *notify = static_cast<EventPacketNotify *>(pnotify);
   
   if(mConnectionParameters.mDebugObjectSizes)
      bstream->writeInt(DebugChecksum, 32);

   EventNote *packQueueHead = NULL, *packQueueTail = NULL;

   while(mUnorderedSendEventQueueHead)
   {
      if(bstream->isFull())
         break;
      // get the first event
      EventNote *ev = mUnorderedSendEventQueueHead;

      bstream->writeFlag(true);
      S32 start = bstream->getBitPosition();

      if(mConnectionParameters.mDebugObjectSizes)
         bstream->advanceBitPosition(BitStreamPosBitSize);
      
      S32 classId = ev->mEvent->getClassId(getNetClassGroup());
      bstream->writeInt(classId, mEventClassBitSize);

      ev->mEvent->pack(this, bstream);
      TNLLogMessageV(LogEventConnection, ("EventConnection %s: WroteEvent %s - %d bits", getNetAddressString(), ev->mEvent->getDebugName(), bstream->getBitPosition() - start));

      if(mConnectionParameters.mDebugObjectSizes)
         bstream->writeIntAt(bstream->getBitPosition(), BitStreamPosBitSize, start);

      if(bstream->getBitSpaceAvailable() < MinimumPaddingBits)
      {
         // rewind to before the event, and break out of the loop:
         bstream->setBitPosition(start - 1);
         bstream->clearError();
         break;
      }

      // dequeue the event and add this event onto the packet queue
      mUnorderedSendEventQueueHead = ev->mNextEvent;
      ev->mNextEvent = NULL;

      if(!packQueueHead)
         packQueueHead = ev;
      else
         packQueueTail->mNextEvent = ev;
      packQueueTail = ev;
   }
   
   bstream->writeFlag(false);   
   S32 prevSeq = -2;
   
   while(mSendEventQueueHead)
   {
      if(bstream->isFull())
         break;
      
      // if the event window is full, stop processing
      if(mSendEventQueueHead->mSeqCount > mLastAckedEventSeq + 126)
         break;

      // get the first event
      EventNote *ev = mSendEventQueueHead;
      S32 eventStart = bstream->getBitPosition();

      bstream->writeFlag(true);

      if(!bstream->writeFlag(ev->mSeqCount == prevSeq + 1))
         bstream->writeInt(ev->mSeqCount, 7);
      prevSeq = ev->mSeqCount;

      if(mConnectionParameters.mDebugObjectSizes)
         bstream->advanceBitPosition(BitStreamPosBitSize);

      S32 start = bstream->getBitPosition();

      S32 classId = ev->mEvent->getClassId(getNetClassGroup());
      bstream->writeInt(classId, mEventClassBitSize);
      ev->mEvent->pack(this, bstream);

      ev->mEvent->getClassRep()->addInitialUpdate(bstream->getBitPosition() - start);
      TNLLogMessageV(LogEventConnection, ("EventConnection %s: WroteEvent %s - %d bits", getNetAddressString(), ev->mEvent->getDebugName(), bstream->getBitPosition() - start));

      if(mConnectionParameters.mDebugObjectSizes)
         bstream->writeIntAt(bstream->getBitPosition(), BitStreamPosBitSize, start - BitStreamPosBitSize);

      if(bstream->getBitSpaceAvailable() < MinimumPaddingBits)
      {
         // rewind to before the event, and break out of the loop:
         bstream->setBitPosition(eventStart);
         bstream->clearError();
         break;
      }

      // dequeue the event:
      mSendEventQueueHead = ev->mNextEvent;      
      ev->mNextEvent = NULL;
      if(!packQueueHead)
         packQueueHead = ev;
      else
         packQueueTail->mNextEvent = ev;
      packQueueTail = ev;
   }
   for(EventNote *ev = packQueueHead; ev; ev = ev->mNextEvent)
      ev->mEvent->notifySent(this);
      
   notify->eventList = packQueueHead;
   bstream->writeFlag(0);
}

void EventConnection::readPacket(BitStream *bstream)
{
   Parent::readPacket(bstream);

   if(mConnectionParameters.mDebugObjectSizes)
   {
      U32 sum = bstream->readInt(32);
      TNLAssert(sum == DebugChecksum, "Invalid checksum.");
   }
   
   S32 prevSeq = -2;
   EventNote **waitInsert = &mWaitSeqEvents;
   bool unguaranteedPhase = true;
   
   while(true)
   {
      bool bit = bstream->readFlag();
      if(unguaranteedPhase && !bit)
      {
         unguaranteedPhase = false;
         bit = bstream->readFlag();
      }
      if(!unguaranteedPhase && !bit)
         break;
      
      S32 seq = -1;
      
      if(!unguaranteedPhase) // get the sequence
      {
         if(bstream->readFlag())
            seq = (prevSeq + 1) & 0x7f;
         else
            seq = bstream->readInt(7);
         prevSeq = seq;
      }

      U32 endingPosition;
      if(mConnectionParameters.mDebugObjectSizes)
         endingPosition = bstream->readInt(BitStreamPosBitSize);

      U32 classId = bstream->readInt(mEventClassBitSize);
      if(classId >= mEventClassCount)
      {
         setLastError("Invalid packet.");
         return;
      }
      NetEvent *evt = (NetEvent *) Object::create(getNetClassGroup(), NetClassTypeEvent, classId);
      if(!evt)
      {
         setLastError("Invalid packet.");
         return;
      }

      // check if the direction this event moves is a valid direction.
      if(   (evt->getEventDirection() == NetEvent::DirUnset)
         || (evt->getEventDirection() == NetEvent::DirServerToClient && isConnectionToClient())
         || (evt->getEventDirection() == NetEvent::DirClientToServer && isConnectionToServer()) )
      {
         setLastError("Invalid Packet.");
         return;
      }


      evt->unpack(this, bstream);
      if(mErrorBuffer[0])
         return;

      if(mConnectionParameters.mDebugObjectSizes)
      {
         TNLAssert(endingPosition == bstream->getBitPosition(),
            avar("unpack did not match pack for event of class %s.",
            evt->getClassName()) );
      }

      if(unguaranteedPhase)
      {
         processEvent(evt);
         delete evt;
         if(mErrorBuffer[0])
            return;
         continue;
      }
      seq |= (mNextRecvEventSeq & ~0x7F);
      if(seq < mNextRecvEventSeq)
         seq += 128;
      
      EventNote *note = mEventNoteChunker.alloc();
      note->mEvent = evt;
      note->mSeqCount = seq;
      TNLLogMessageV(LogEventConnection, ("EventConnection %s: RecvdGuaranteed %d", getNetAddressString(), seq));

      while(*waitInsert && (*waitInsert)->mSeqCount < seq)
         waitInsert = &((*waitInsert)->mNextEvent);
      
      note->mNextEvent = *waitInsert;
      *waitInsert = note;
      waitInsert = &(note->mNextEvent);
   }
   while(mWaitSeqEvents && mWaitSeqEvents->mSeqCount == mNextRecvEventSeq)
   {
      mNextRecvEventSeq++;
      EventNote *temp = mWaitSeqEvents;
      mWaitSeqEvents = temp->mNextEvent;
      
      TNLLogMessageV(LogEventConnection, ("EventConnection %s: ProcessGuaranteed %d", getNetAddressString(), temp->mSeqCount));
      processEvent(temp->mEvent);
      mEventNoteChunker.free(temp);
      if(mErrorBuffer[0])
         return;
   }
}

bool EventConnection::postNetEvent(NetEvent *theEvent)
{   
   S32 classId = theEvent->getClassId(getNetClassGroup());
   if(U32(classId) >= mEventClassCount && getConnectionState() == Connected)
      return false;

   theEvent->notifyPosted(this);

   EventNote *event = mEventNoteChunker.alloc();
   event->mEvent = theEvent;
   event->mNextEvent = NULL;

   if(event->mEvent->mGuaranteeType == NetEvent::GuaranteedOrdered)
   {
      event->mSeqCount = mNextSendEventSeq++;
      if(!mSendEventQueueHead)
         mSendEventQueueHead = event;
      else
         mSendEventQueueTail->mNextEvent = event;
      mSendEventQueueTail = event;
   }
   else
   {
      event->mSeqCount = InvalidSendEventSeq;
      if(!mUnorderedSendEventQueueHead)
         mUnorderedSendEventQueueHead = event;
      else
         mUnorderedSendEventQueueTail->mNextEvent = event;
      mUnorderedSendEventQueueTail = event;
   }
   return true;
}

bool EventConnection::isDataToTransmit()
{
   return mUnorderedSendEventQueueHead || mSendEventQueueHead || Parent::isDataToTransmit();
}

};

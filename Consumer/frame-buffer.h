/*
 * frame-buffer.h
 *
 *  Created on: Jun 15, 2016
 *      Author: xyh
 */

#ifndef FRAME_BUFFER_H_
#define FRAME_BUFFER_H_

#include <functional>
#include <queue>
#include <vector>
#include <iostream>
//#include <boost/thread/mutex.hpp>
#include <thread>
#include <mutex>

#include <ndn-cpp/name.hpp>
#include <ndn-cpp/data.hpp>

using namespace std;
using namespace ndn;

class FrameBuffer
{
public:
	class Slot
	{
	public:

		class Comparator
		{
		public:
			Comparator(bool inverted = false):inverted_(inverted){}

			bool operator() (const Slot* slot1, const Slot* slot2)
			{
				return slot1->slotNumber_ < slot2->slotNumber_;
			}
/*
			bool operator < (Slot* slot1, Slot* slot2)
			{
				return slot1->slotNumber_ < slot2->slotNumber_;
			}
*/
		private:
			bool inverted_;

		};

		Slot( unsigned int frameSize );
		~Slot();

		unsigned int getFrameSize()
		{ return frameSize_; }

		void setDataPtr ( unsigned char* dataPtr )
		{ dataPtr_ = dataPtr; }

		unsigned char* getDataPtr ()
		{ return dataPtr_; }

		void setSlotPrefix ( ndn::Name prefix )
		{ slotPrefix_ = prefix; }

		void setSlotNumber ( int64_t slotNumber )
		{ slotNumber_ = slotNumber; }

	private:
		unsigned int frameSize_ = 0;
		//unsigned int allocatedSize_ = 0;
		//int64_t requestTimestamp_, recvTimestamp_;
		unsigned char* dataPtr_ = NULL;

		ndn::Name slotPrefix_;
		int64_t slotNumber_;

	};

	FrameBuffer(){}
	~FrameBuffer(){}

	void addFrame(const ndn::ptr_lib::shared_ptr<Data>& data);
	Slot* getFrame();

private:
	typedef
	priority_queue< Slot*, vector<Slot*>, Slot::Comparator/*greater<Slot::Comparator>*/ >
	PriorityQueue;

	PriorityQueue priorityQueue_;
	std::recursive_mutex syncMutex_;

};


#endif /* FRAME_BUFFER_H_ */

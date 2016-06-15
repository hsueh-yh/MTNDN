/*
 * frame-buffer.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: xyh
 */

#include <iostream>
#include "frame-buffer.h"


FrameBuffer::Slot::Slot( unsigned int frameSize ):
	frameSize_(frameSize)
{}


void FrameBuffer::addFrame(const ndn::ptr_lib::shared_ptr<Data>& data)
{
	Slot *newData = new Slot(data->getContent ().size ());
	newData->setDataPtr( (unsigned char*)data->getContent().buf() );
	newData->setSlotPrefix( data->getName() );

	priorityQueue_.push(newData);

}


FrameBuffer::Slot* FrameBuffer::getFrame()
{
	FrameBuffer::Slot*  tmp = priorityQueue_.top();
	priorityQueue_.pop();
	return tmp;
}

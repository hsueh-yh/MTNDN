
#include <math.h>

#include "frame-buffer.h"
#include "utils.h"
#include "name-components.h"
#include "namespacer.h"

FrameBuffer::FrameBuffer() :
    maxSegmentSize_(ndn::Face::getMaxNdnPacketSize()),
    maxSegBlockSize_(maxSegmentSize_-SegmentData::getHeaderSize()),
    bufSize_(200)
{
    init();
}

FrameBuffer::~FrameBuffer()
{}

void
FrameBuffer::init(int frameNumbers)
{
    if( bufSize_ < frameNumbers )
        bufSize_ = frameNumbers;
    reset();
    initSlots();
}

void
FrameBuffer::recvFrame( FrameData &frame, unsigned int frameNo )
{
    boost::shared_ptr<SegmentData> segment;
    int size = frame.size();
    ndn::Name stmPrefix = publisher_->getStreamVideoPrefix();

    if( size <= maxSegBlockSize_ )
    {
        segment = getFreeSlot();
        segment->fillSegmentData(frame.getData(),size);
        SegmentData::segmentDataFromRaw(size,frame.getData(),*segment);
        ndn::Name segPrefix(stmPrefix);
        segPrefix.append(NdnRtcUtils::componentFromInt(frameNo));
        segPrefix.append(NdnRtcUtils::componentFromInt(0));
        activeSlots_[segPrefix] = segment;
    }
    else
    {
        int segNum = ceil( (double)size / (double)maxSegBlockSize_ );
        int lastSegSize = size % maxSegBlockSize_;
        for( int i = 0; i < segNum; i++ )
        {
            segment = getFreeSlot();
            segment->fillSegmentData(frame.getData()+(i*maxSegBlockSize_),
                                     i==segNum-1 ? lastSegSize : maxSegBlockSize_);

            ndn::Name segPrefix(stmPrefix);
            segPrefix.append(NdnRtcUtils::componentFromInt(frameNo));
            segPrefix.append(NdnRtcUtils::componentFromInt(i));
            activeSlots_[segPrefix] = segment;
        }
    }
}

bool
FrameBuffer::requestSegment( ndn::Interest& interest, SegmentData& segData )
{
    uint32_t nonce = NdnRtcUtils::blobToNonce(interest.getNonce());
    std::map<ndn::Name, boost::shared_ptr<SegmentData> >::iterator iter;
    iter = activeSlots_.find(interest.getName());
    if (iter==activeSlots_.end())
        return false;
    boost::shared_ptr<SegmentData> segment = iter->second;
    segData.copyFromRaw(segment->size(),segment->getData());
    return true;
}


//protected functions
//**************************************************
void
FrameBuffer::reset()
{
    boost::shared_ptr<SegmentData> segment;
    std::map<ndn::Name, boost::shared_ptr<SegmentData> >::iterator it;
    for( it = activeSlots_.begin(); it != activeSlots_.end(); ++it )
    {
        segment =it->second;
        freeSlots_.push_back(segment);
    }
    activeSlots_.clear();
}

void
FrameBuffer::initSlots()
{
    while( freeSlots_.size() < bufSize_ )
    {
        boost::shared_ptr<SegmentData> segment;
        segment.reset(new SegmentData(maxSegmentSize_));
        freeSlots_.push_back(segment);
    }
}

boost::shared_ptr<SegmentData>
FrameBuffer::getFreeSlot()
{
    boost::shared_ptr<SegmentData> segment;
    if( freeSlots_.size() )
    {
        segment = freeSlots_.at(freeSlots_.size()-1);
        freeSlots_.pop_back();
    }
    else
    {
        std::map<ndn::Name, boost::shared_ptr<SegmentData> >::iterator it;
        it = activeSlots_.end();
        segment = (--it)->second;
        activeSlots_.erase(it);
    }
    return segment;
}

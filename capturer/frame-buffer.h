#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <boost/thread/mutex.hpp>
#include <vector>
#include <map>

#include <ndn-cpp/name.hpp>
#include <ndn-cpp/face.hpp>

#include "frame-data.h"
#include "publisher.h"

class Publisher;

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();
    void init( int frameNumbers = 200 );

    void recvFrame( FrameData &frame, unsigned int frameNo );

    bool requestSegment( ndn::Interest& interest, SegmentData& segData );

private:
    void reset();

    void initSlots();

    boost::shared_ptr<SegmentData> getFreeSlot();

    boost::shared_ptr<Publisher> publisher_;

    std::vector<boost::shared_ptr<SegmentData> > freeSlots_;
    std::map<ndn::Name, boost::shared_ptr<SegmentData> > activeSlots_;

    int     maxSegBlockSize_,
            maxSegmentSize_;
    int     bufSize_;

};


#endif // FRAMEBUFFER_H

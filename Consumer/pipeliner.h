/*
 * Pipeliner.h
 *
 *  Created on: Jun 15, 2016
 *      Author: xyh
 */

#ifndef PIPELINER_H_
#define PIPELINER_H_

#include <ndn-cpp/name.hpp>
#include <ndn-cpp/data.hpp>

#include "frame-buffer.h"

using namespace ndn;

class Pipeliner
{
public:
	Pipeliner(boost::shared_ptr<FrameBuffer> frameBuffer);
	~Pipeliner(){}

	void onData(const ptr_lib::shared_ptr<const Interest>& interest, const ptr_lib::shared_ptr<Data>& data);

	void onTimeout(const ptr_lib::shared_ptr<const Interest>& interest);
/*
	OnData
	getOnDataHandler()
	{ return bind(&Pipeliner::onData, boost::dynamic_pointer_cast<Pipeliner>(shared_from_this()), _1, _2); }

	OnTimeout
	getOnTimeoutHandler()
	{ return bind(&Pipeliner::onTimeout, boost::dynamic_pointer_cast<Pipeliner>(shared_from_this()), _1); }
*/

private:
	boost::shared_ptr<FrameBuffer> frameBuffer_;

};


#endif /* PIPELINER_H_ */

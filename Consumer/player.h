/*
 * player.h
 *
 *  Created on: Jun 16, 2016
 *      Author: xyh
 */


#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "decoder.h"
#include "frame-buffer.h"


class Player
{
public:
	Player(boost::shared_ptr<FrameBuffer> frameBuffer);

	~Player();

	bool init ();

	void writeFile ();

private:
	boost::shared_ptr<FrameBuffer> frameBuffer_;
	FILE *pFile_, *pFile1_;
	Decoder *decoder_;
};


#endif /* PLAYER_H_ */

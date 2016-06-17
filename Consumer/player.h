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

#include "frame-buffer.h"


class Player
{
public:
	Player(boost::shared_ptr<FrameBuffer> frameBuffer):
		frameBuffer_(frameBuffer)
{
		pf = fopen ( "consumer.264", "wb+" );
		if ( pf == NULL )
		{
			std::cout << "open consumer.264 error" << std::endl;
			return;
		}
}

	~Player()
	{
		fclose(pf);
	}

	void writeFile ()
	{
		std::cout<< std::endl<< std::endl << " Write start " << std::endl<< std::endl;
		//cout << endl << "start write" << endl<< endl<< endl;
		int i=0;
		std::cout << i <<std::endl;
		//for ( int i = 0; i < 200; i++ )
		while( i < 900)
		{
			std::cout << " Write " << ++i << " ";
			FrameBuffer::Slot *slot =NULL;
			while ( slot== NULL )
				slot = frameBuffer_->getFrame();

			std::cout  << slot->getFrameSize() << std::endl;
			fwrite ( slot->getDataPtr(), slot->getFrameSize(), 1, pf );
		}
	}

private:
	boost::shared_ptr<FrameBuffer> frameBuffer_;
	FILE *pf;
};


#endif /* PLAYER_H_ */

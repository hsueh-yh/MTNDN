/*
 * player.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: xyh
 */

#include "player.h"


Player::Player(boost::shared_ptr<FrameBuffer> frameBuffer):
frameBuffer_(frameBuffer)
{
	pf = fopen ( "consumer.264", "wb+" );
	if ( pf == NULL )
	{
		std::cout << "open consumer.264 error" << std::endl;
		return;
	}
}


Player::~Player()
{
	fclose(pf);
}


void Player::writeFile ()
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

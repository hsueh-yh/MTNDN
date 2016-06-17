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
	Player(boost::shared_ptr<FrameBuffer> frameBuffer);

	~Player();

	void writeFile ();

private:
	boost::shared_ptr<FrameBuffer> frameBuffer_;
	FILE *pf;
};



/*
#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "Decoder/decoder.h"
#include "frame-buffer.h"


class Player
{
public:
	Player(boost::shared_ptr<FrameBuffer> frameBuffer):
		frameBuffer_(frameBuffer),
		decoder_(new Decoder())
	{

		pFile_ = fopen ( "consumer.264", "wb+" );
		if ( pFile_ == NULL )
		{
			std::cout << "open consumer.264 error" << std::endl;
			return;
		}
	}

	~Player()
	{
		fclose(pFile_);
		decoder_->StopDecoder();
		decoder_->ReleaseConnection();
	}

	bool init ()
	{
		if (!decoder_->InitDeocder(640, 480))
		{
			return false;
		}

		return true;
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

			std::cout << "Size: " << slot->getFrameSize();// << std::endl;
			unsigned char *p_Out_Frame =
					new unsigned char[640 * 480 * 3 / 2];
			int outlen;

			decoder_->decode(slot->getDataPtr(),slot->getFrameSize(), p_Out_Frame, outlen );
			std::cout << " Size: " << outlen << std::endl;
			if ( outlen > 0 )
				fwrite ( p_Out_Frame, outlen, 1, pFile_ );
		}
	}

private:
	boost::shared_ptr<FrameBuffer> frameBuffer_;
	FILE *pFile_;
	Decoder *decoder_;
};
*/

#endif /* PLAYER_H_ */

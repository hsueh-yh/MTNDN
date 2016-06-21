/*
 * player.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: xyh
 */

#include "player.h"


static int FindStartCode(unsigned char *Buf, int zeros_in_startcode)
{
	int info;
	int i;

	info = 1;
	for (i = 0; i < zeros_in_startcode; i++)
	{
		if (Buf[i] != 0)
			info = 0;
	}

	if (Buf[i] != 1)
		info = 0;
	return info;
}

static bool Check_StartCode(unsigned char *Buf, int pos)
{
	int info3 = 0;

	info3 = FindStartCode(&Buf[pos - 4], 3);
	return info3 == 1;

}

static int getNextNal(unsigned char* &inpf, unsigned char* inpf_end, unsigned char* inBuf)
{
	int inBuf_pos = 0;
	int StartCodeFound = 0;
	int info2 = 0;
	int info3 = 0;

	int nCount = 0;
	while ( inpf <= inpf_end && ++nCount <= 4)
	{
		inBuf[inBuf_pos++] = *inpf++;
	}

	if (!Check_StartCode(inBuf, inBuf_pos))
	{
		return 0;
	}


	//while (!feof(inpf) && (inBuf[inBuf_pos++] = fgetc(inpf)) == 0);

	//find the next start code
	while (!StartCodeFound)
	{
		//end of file
		if (inpf>=inpf_end)
		{
			//			return -1;
			return inBuf_pos - 1;
		}
		inBuf[inBuf_pos++] = *inpf++;

		StartCodeFound = Check_StartCode(inBuf, inBuf_pos);
	}

	//fseek(inpf, -4, SEEK_CUR);
	inpf -= 4;

	// return the end(length) of this NALU
	return inBuf_pos - 4;
}



Player::Player(boost::shared_ptr<FrameBuffer> frameBuffer):
frameBuffer_(frameBuffer),
decoder_(new Decoder())
{
	pFile_ = fopen ( "consumer.yuv", "wb+" );
	if ( pFile_ == NULL )
	{
		std::cout << "open consumer.yuv error" << std::endl;
		return;
	}

	pFile1_ = fopen ( "testPlayer.264", "wb+" );
	if ( pFile1_ == NULL )
	{
		std::cout << "open consumer.yuv error" << std::endl;
		return;
	}

}

Player::~Player()
{
	fclose(pFile_);
	fclose(pFile1_);
	decoder_->StopDecoder();
	decoder_->ReleaseConnection();
}


bool Player::init ()
{
	FrameBuffer::Slot *slot1 =NULL, *slot2 =NULL;
	uint8_t *sps, *pps;
	int spslen,ppslen;

	while ( slot1== NULL )
		slot1 = frameBuffer_->getFrame();

	sps = slot1->getDataPtr();
	spslen = slot1->getFrameSize();

	while ( slot2== NULL )
			slot2 = frameBuffer_->getFrame();

	pps = slot2->getDataPtr();
	ppslen = slot2->getFrameSize();

	cout << "spslen" << spslen << "ppslen " << ppslen << endl;

	if (!decoder_->InitDeocder(640, 480, sps, spslen, pps, ppslen))
	{
		return false;
	}

	return true;
}


void Player::writeFile ()
{
	init();
	//sleep(3);
	std::cout<< std::endl<< std::endl << " Write start " << std::endl<< std::endl;
	//cout << endl << "start write" << endl<< endl<< endl;
	int i=0;
	std::cout << i <<std::endl;
	//for ( int i = 0; i < 200; i++ )
	while( ++i <= 202)
	{

		FrameBuffer::Slot *slot =NULL;
		//cout << frameBuffer_->status_ << endl;
		//while(frameBuffer_->status_ != STARTED);

		while ( slot== NULL )
			slot = frameBuffer_->getFrame();

		unsigned char *p_Out_Frame = new unsigned char[640 * 480 * 3 / 2];
		unsigned char *p_In_Frame = slot->getDataPtr();
		int outlen, inlen;
		inlen = slot->getFrameSize();

//		std::cout << "Write " << i << " " << "size:" << inlen<< std::endl;
//		cout << slot->getSlotNumber()<<endl;
		for( int i = 0; i <20; i++ )
				printf("%X ",p_In_Frame[i]);
		cout << endl;
//		std::cout << std::endl << std::endl;
//		fwrite ( p_In_Frame, inlen, 1, pFile1_ );

		std::cout << std::endl << "SizeIn: " << slot->getFrameSize() << std::endl;

		decoder_->decode( p_In_Frame, inlen, p_Out_Frame, outlen );
		//decoder_->decode( pFile1_,slot->getDataPtr(),slot->getFrameSize(), p_Out_Frame, outlen );

		std::cout << std::endl << "SizeOut: " << outlen << std::endl;

		if ( outlen > 0 )
			fwrite ( p_Out_Frame, outlen, 1, pFile_ );

		usleep(300);
	}
	cout << endl << "wirte thread end" << endl;
}
/*
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
}*/

#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <map>

#include <ndn-cpp/face.hpp>
#include <ndn-cpp/security/key-chain.hpp>

#include "frame-data.h"


// #define WIDTH 640
// #define HEIGHT 480
#define WIDTH 1080
#define HEIGHT 720


using namespace ndn;
using namespace std;


class Publisher {
public:

	Publisher ( KeyChain &keyChain, const Name& certificateName );

	~Publisher ();

	bool init ();

	void view();

	// onInterest.
	void operator()
					(const ptr_lib::shared_ptr<const Name>& prefix,
					const ptr_lib::shared_ptr<const Interest>& interest, Face& face,
					uint64_t interestFilterId,
					const ptr_lib::shared_ptr<const InterestFilter>& filter);

	// onRegisterFailed.
	void operator()(const ptr_lib::shared_ptr<const Name>& prefix);


	int start();
	int stop();


	KeyChain keyChain_;
	//Name myPrefix;
	Name certificateName_;
	int responseCount_;

	int stat;

	FILE* ifp;
	FILE* outfp;
	int count;

	unsigned char *spsBuf_, *ppsBuf_;
	long frameStart_;

	map<int,FrameData*> mapRep_;
	unsigned char* repertory_;
	long frameCount_;

	unsigned char* tmpbuf_;


public:
	static int FindStartCode ( unsigned char *Buf, int zeros_in_startcode )
	{
		int info;
		int i;

		info = 1;
		for ( i = 0; i < zeros_in_startcode; i++ )
		{
			if ( Buf[i] != 0 )
				info = 0;
		}

		if ( Buf[i] != 1 )
			info = 0;
		return info;
	}

	static bool Check_StartCode ( unsigned char *Buf, int pos )
	{
		int info3 = 0;

		info3 = FindStartCode ( &Buf[pos - 4], 3 );
		return info3 == 1;

	}

	static int getNextNal ( FILE* inpf, unsigned char* Buf )
	{
		int pos = 0;
		int StartCodeFound = 0;
		int info2 = 0;
		int info3 = 0;

		int nCount = 0;
		while ( !feof ( inpf ) && ++nCount <= 4 )
		{
			Buf[pos++] = fgetc ( inpf );
		}

		if ( !Check_StartCode ( Buf, pos ) )
		{
			return 0;
		}


		while ( !feof ( inpf ) && ( Buf[pos++] = fgetc ( inpf ) ) == 0 );

		//find the next start code
		while ( !StartCodeFound )
		{
			//end of file
			if ( feof ( inpf ) )
			{
				//			return -1;
				return pos - 1;
			}
			Buf[pos++] = fgetc ( inpf );

			StartCodeFound = Check_StartCode ( Buf, pos );
		}

		fseek ( inpf, -4, SEEK_CUR );

		// return the end(length) of this NALU
		return pos - 4;
	}


};

#endif	//_FRAME_BUFFER_H_

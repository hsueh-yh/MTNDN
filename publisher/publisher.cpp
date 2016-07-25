
#include <arpa/inet.h>

#include "publisher.h"


#define REQUEST_FIRST_FRAME_ 0

Publisher::Publisher ( KeyChain &keyChain, const Name& certificateName ) :
		keyChain_ ( keyChain ),
		certificateName_ ( certificateName ),
		responseCount_ ( 0 ),
		count ( 0 ),
		spsBuf_(NULL),
		ppsBuf_(NULL),
		frameStart_(0),
		frameCount_(0),
		tmpbuf_(new uint8_t[WIDTH * HEIGHT * 3 / 2]),
		tmpLen(0),
		spsppsBuf_(NULL),
		spsppsLen(0)

{
	ifp = fopen ( "out.264", "rb" );
	if ( ifp == NULL )
		cout << "Open file error!" << endl;


/*	outfp = fopen ( "copyout.264", "wb" );
	if ( ifp == NULL )
		cout << "Open file error!" << endl;
*/
}


Publisher::~Publisher ()
{
	fclose ( ifp );
	//fclose ( outfp );
}


bool
Publisher::init()
{
	long fileLen;
	fseek(ifp, 0, SEEK_END);
	fileLen = ftell(ifp);

	repertory_ = (unsigned char*) malloc (fileLen);
	fseek(ifp, 0, SEEK_SET);

	int nalLength = 0, id = -1;
	unsigned char *ptmp = repertory_;

	// read sps and pps
	unsigned char *spspps = NULL;
	FrameData *spsppsframe = (FrameData*) malloc (sizeof(FrameData));
	while( !feof(ifp) && frameCount_ <= 1 )
	{
		nalLength = getNextNal ( ifp, ptmp );
		if ( nalLength == 0 )
			continue;
		if ( frameCount_ == 0 )
		{
			spsppsLen += nalLength;
			spsppsframe->header_.length_ = spsppsLen;
			spsppsframe->header_.completeFrame_ = false;
			spsppsframe->header_.encodedHeight_ = HEIGHT;
			spsppsframe->header_.encodedWidth_ = WIDTH;
			spsppsframe->header_.frameType_ = ParaSet;
			spsppsframe->header_.frameNumber = 0;
			spsppsframe->buf_ = ptmp;
			spsppsBuf_ =ptmp;

			ptmp += nalLength;
			frameCount_++;
		}
		else
		{
			spsppsLen += nalLength;
			spsppsframe->header_.length_ = spsppsLen;
			ptmp += nalLength;
			frameCount_++;
		}
	}
	frameCount_ = 0;
	mapRep_.insert(pair<int, FrameData*>(id, spsppsframe));
	id++;

	// read frames
	while ( !feof(ifp) )
	{
		nalLength = getNextNal ( ifp, ptmp );
		if ( nalLength == 0 )
			continue;
		else
		{
			FrameData *frame = (FrameData*) malloc (sizeof(FrameData));
			frame->header_.length_ = nalLength;
			frame->header_.completeFrame_ = true;
			frame->header_.encodedHeight_ = HEIGHT;
			frame->header_.encodedWidth_ = WIDTH;
			frame->header_.frameType_ = IFrame;
			frame->header_.frameNumber = frameCount_;
			frame->buf_ = ptmp;
			mapRep_.insert(pair<int, FrameData*>(id, frame));
			frameCount_++;

			++id;
			ptmp += nalLength;
		}
	}

	cout << "Init mapRepSize: " << mapRep_.size()
		 << ", FrameCount: " << frameCount_
		 << ", Repertory: 0 - " << ptmp - repertory_
		 << ", SPS&PPS: " << spsppsLen << " Byte"
		 << endl << endl;

	//view();

	if (feof(ifp))
		return true;
	else
		return false;
}

void Publisher::view()
{
	FrameData *pframe;

	cout << "SPS and PPS : " << spsppsLen <<endl;
	for( int i = 0; i < spsppsLen; i++ )
	{
		printf("%2X ",spsppsBuf_[i]);
	}
	cout << endl << endl;

	for ( int i = 0; i < frameCount_; i++ )
	{
		map<int, FrameData*>::iterator iter;
		iter = mapRep_.find(i);
		if( iter != mapRep_.end())
		{
			pframe = iter->second;
		}
		else
		{
			cout << "Deny" << endl;
			return;
		}

		cout << i << " "
			 << sizeof(FrameDataHeader) << "+"
			 <<pframe->header_.length_<<endl;

		//printf("%d %d\n",i, pframe->header_.length_);
		for( int i = 0; (i < 40 && i < pframe->header_.length_); i++ )
			printf("%2X ",pframe->buf_[i]);
		cout << endl << endl;
	}
}


// onInterest.
void Publisher::operator()
						(	const ptr_lib::shared_ptr<const Name>& prefix,
							const ptr_lib::shared_ptr<const Interest>& interest, 
							Face& face,
							uint64_t interestFilterId,
							const ptr_lib::shared_ptr<const InterestFilter>& filter )
{
	++responseCount_;
	++count;
	//cout << "Got an interest..." << endl;


	Name requestName(interest->getName());
	int componentCount = requestName.getComponentCount();
	string framenoStr = requestName.get(componentCount-1).toEscapedString();

	long requestNo = std::atoi(framenoStr.c_str());
	long responseNo = requestNo % frameCount_;

	cout << "Request : " << requestNo
			<< " by " << requestName.to_uri() << endl;
	
	
	FrameData *pframe;
	map<int, FrameData*>::iterator iter;
	iter = mapRep_.find(responseNo);
	

	if( iter != mapRep_.end())
	{
		pframe = iter->second;
	}
	else
	{
		cout << "Deny" << endl << endl;
		return;
	}


	if( requestNo == REQUEST_FIRST_FRAME_ )
	{
		FrameDataHeader respFram;

		memcpy( &respFram, pframe, sizeof(FrameDataHeader));

		tmpLen = sizeof(FrameDataHeader) + respFram.length_ + spsppsLen;
		respFram.length_ += spsppsLen;

		memcpy(tmpbuf_, &respFram, sizeof(FrameDataHeader));	//copy frame header
		memcpy(tmpbuf_ + sizeof(FrameDataHeader), spsppsBuf_, spsppsLen);	//copy sps and pps
		memcpy(tmpbuf_ + sizeof(FrameDataHeader)+spsppsLen, pframe->buf_, pframe->header_.length_ );	//copy frame data
	}
	else
	{
		tmpLen = sizeof(FrameDataHeader) + pframe->header_.length_;

		memcpy(tmpbuf_, pframe, sizeof(FrameDataHeader));	//copy frame header
		memcpy(tmpbuf_ + sizeof(FrameDataHeader), pframe->buf_, pframe->header_.length_ );	//copy frame data
	}

	const Blob content ( tmpbuf_, tmpLen );

	// Make and sign a Data packet.
	Data data(requestName);

	data.setContent( content );
	
	keyChain_.sign(data, certificateName_);

	cout << "Response: " << requestNo
		 << " by " << responseNo
		 << " size:"
		 << sizeof(FrameDataHeader) << "+"<<pframe->header_.length_ << "="
		 << content.size () << endl << endl;

	face.putData(data);

//	for( int i = 0; i <30; i++ )
//		printf("%2X ",data.getContent().buf()[i]);
//	cout << endl << endl;

	//fwrite(data.getContent().buf(), data.getContent().size(),1,outfp);
}


// onRegisterFailed.
void Publisher::operator()(const ptr_lib::shared_ptr<const Name>& prefix )
{
	++responseCount_;
	cout << "Register failed for prefix " << prefix->toUri() << endl;
}


int Publisher::start()
{
	stat = 1;

}


int Publisher::stop()
{
	stat = 0;
}

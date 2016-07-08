
#include <arpa/inet.h>

#include "publisher.h"


Publisher::Publisher ( KeyChain &keyChain, const Name& certificateName ) :
		keyChain_ ( keyChain ),
		certificateName_ ( certificateName ),
		responseCount_ ( 0 ),
		count ( 0 ),
		spsBuf_(NULL),
		ppsBuf_(NULL),
		frameStart_(0),
		frameCount_(0),
		tmpbuf_(new uint8_t[WIDTH * HEIGHT * 3 / 2])
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

	int nalLength = 0, id = 0;
	unsigned char *ptmp = repertory_;
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
			frame->header_.encodedHeight_ = 480;
			frame->header_.encodedWidth_ = 640;
			frame->header_.frameType_ = IFrame;
			frame->header_.frameNumber = frameCount_;
			frame->buf_ = ptmp;
			mapRep_.insert(pair<int, FrameData*>(id, frame));
			frameCount_++;
		}
		++id;
		ptmp += nalLength;
	}

	cout << "Init mapRep: " << mapRep_.size()
		 << " frameCount: " << frameCount_
		 << " repertory: 0 - " << ptmp - repertory_ << endl;

	if (feof(ifp))
		return true;
	else
		return false;

	/*
	int nalLenth = 0,spsOk = 0, ppsOk = 0;

	spsBuf_ = new unsigned char[20];
	ppsBuf_ = new unsigned char[20];

	//get sps
	while ( !feof(ifp) )
	{
		nalLenth = getNextNal ( ifp, spsBuf_ );
		if ( nalLenth == 0 )
			continue;
		else
		{
			spsOk = 1;
			break;
		}
	}

	//get pps
	while ( !feof(ifp) )
	{
		nalLenth = getNextNal ( ifp, ppsBuf_ );
		if ( nalLenth == 0 )
			continue;
		else
		{
			ppsOk = 1;
			break;
		}
	}

	if(spsOk && ppsOk)
	{
		frameStart_ = ftell(ifp);
		return true;
	}

	return false;
	*/
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
	string framenoStr = requestName.get(1).toEscapedString();
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
		cout << "Deny" << endl;
		return;
	}

	size_t tmpLen = sizeof(FrameDataHeader) + pframe->header_.length_;
	//unsigned char* tmp = (unsigned char* ) malloc (tmpLen);

	memcpy(tmpbuf_, pframe, sizeof(FrameDataHeader));	//copy frame header
	memcpy(tmpbuf_ + sizeof(FrameDataHeader), pframe->buf_, pframe->header_.length_ );	//copy frame data


	const Blob content ( tmpbuf_, tmpLen );

	// Make and sign a Data packet.
	Data data(requestName);

	data.setContent( content );
	
	keyChain_.sign(data, certificateName_);

	cout << "Response: " << requestNo
		 << " size:"
		 << sizeof(FrameDataHeader) << "+"<<pframe->header_.length_ << "="
		 << content.size () << endl;

	face.putData(data);

	for( int i = 0; i <30; i++ )
		printf("%2X ",data.getContent().buf()[i]);
	cout << endl << endl;
	//fwrite(data.getContent().buf(), data.getContent().size(),1,outfp);
}


// onRegisterFailed.
void Publisher::operator()(const ptr_lib::shared_ptr<const Name>& prefix)
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

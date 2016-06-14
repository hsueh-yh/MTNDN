
#include "Publisher.h"


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
	cout << "Got an interest..." << endl;

	// Make and sign a Data packet.
	Data data(interest->getName());
	Name name;
	//string content( string("video") + interest->getName().toUri() );
	//FILE* ifp = fopen ( "out.264", "rb" );
	
	
	uint8_t *p_In_Frame = new uint8_t[WIDTH * HEIGHT * 3 / 2];
	
	int nalLenth;
	
	if ( ifp == NULL )
	{
		cout << "file not opened" << endl;
		return;
	}
	if ( feof ( ifp ) )
	{
		cout << "Read again" << endl;
		fseek ( ifp, 0, SEEK_SET );
	}
	
	while ( !feof(ifp) )
	{
		nalLenth = getNextNal ( ifp, p_In_Frame );
		if ( nalLenth == 0 )
			continue;
		else
			break;
	}
	
	const Blob content ( p_In_Frame, nalLenth );

	cout << "Got data ...  " << endl;

	data.setContent( content );
	
	keyChain_.sign(data, certificateName_);

	cout << "Sent content " << count;
	cout << " size:" << content.size () << endl;
	face.putData(data);
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

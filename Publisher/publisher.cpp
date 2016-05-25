
#include "Publisher.h"


// onInterest.
void Publisher::operator()
(const ptr_lib::shared_ptr<const Name>& prefix,
	const ptr_lib::shared_ptr<const Interest>& interest, Face& face,
	uint64_t interestFilterId,
	const ptr_lib::shared_ptr<const InterestFilter>& filter)
{
	++responseCount_;

	// Make and sign a Data packet.
	Data data(interest->getName());
	string content( string("video") + interest->getName().toUri() );
	data.setContent((const uint8_t *)&content[0], content.size());
	keyChain_.sign(data, certificateName_);

	cout << "Sent content " << content << endl;
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

#include "Consumer.h"


Consumer::Consumer() {
	callbackCount_ = 0;
}


void Consumer::onData(const ptr_lib::shared_ptr<const Interest>& interest, const ptr_lib::shared_ptr<Data>& data)
{
	++callbackCount_;
	cout << "Got data packet with name " << data->getName().toUri() << endl;
	for (size_t i = 0; i < data->getContent().size(); ++i)
		cout << (*data->getContent())[i];
	cout << endl;
}

void Consumer::onTimeout(const ptr_lib::shared_ptr<const Interest>& interest)
{
	++callbackCount_;
	cout << "Time out for interest " << interest->getName().toUri() << endl;
}

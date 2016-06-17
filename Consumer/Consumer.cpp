#include "Consumer.h"


static boost::asio::io_service libIoService;

Consumer::Consumer () :
	//recv_buf ( new CirQueue<frame_buf> ( 20 ) ),
	callbackCount_ ( 0 )
{
	std::cout << "new consumer " << endl;
	NdnRtcUtils::setIoService(libIoService);

	//NdnRtcUtils::performOnBackgroundThread([=]()->void{
			NdnRtcUtils::createLibFace();
	//});
	NdnRtcUtils::startBackgroundThread();

	// Counter holds data used by the callbacks.
	//Consumer consumer(NdnRtcUtils::getLibFace()->getFaceWrapper());
	faceWrapper_ = NdnRtcUtils::getLibFace()->getFaceWrapper();
	//pthread_mutex_init ( &recv_buf_mutex, NULL );
	
	/*
	pf = fopen ( "consumer.264", "wb+" );
	if ( pf == NULL )
	{
		cout << "open consumer.264 error" << endl;
		return;
	}
	*/
}

Consumer::~Consumer ()
{
	//while ( 0 != pthread_mutex_destroy ( &recv_buf_mutex ));
	//fclose ( pf );
}


void Consumer::init()
{
	std::cout << "init consumer " << endl;
	frameBuffer_.reset(new FrameBuffer());
	pipeliner_.reset(new Pipeliner(frameBuffer_));
}


void Consumer::start()
{
	std::cout << "start consumer " << endl;
	try {
			char tmp[20]="/vide1/01";
			Name name;
			cout << "sending interests..." << endl;
			int i = 0;
			//for (int i = 0; i < 200; i++)
			while(++i < 1000)
			{
				//Name name("/video/");

				tmp[8]=i+'0';

				name.set(tmp);
				time_t rawtime;
				time(&rawtime);
				name.appendTimestamp(rawtime);

				//cout << "Express name " << i << " " << name.toUri() << endl;
				// Use bind to pass the counter object to the callbacks.
				faceWrapper_->expressInterest(
						name,
						bind(&Pipeliner::onData, pipeliner_.get(), _1, _2),
						bind(&Pipeliner::onTimeout, pipeliner_.get(), _1));
				usleep(1000);
			}


			//ioservice thread is running
			while(1);
			//sleep(5);

		}
		catch (std::exception& e) {
			cout << "exception: " << e.what() << endl;
		}
}


void Consumer::onData(const ptr_lib::shared_ptr<const Interest>& interest, const ptr_lib::shared_ptr<Data>& data)
{
	/*
	std::cout<<"onData:"<<std::endl;
	++callbackCount_;
	cout << "Got data "<< data->getName().toUri();
	cout << " size: " << data->getContent ().size () << endl;
	if ( data->getContent ().buf () == NULL )
		cout << "content is null !" << endl;

	frame_buf tmpbuf;
	memcpy ( &( tmpbuf.p_In_Frame ), data->getContent ().buf (), data->getContent ().size () );
	tmpbuf.size = data->getContent ().size ();
	
	cout << "write ring buffer... ";
	// write ring buffer
	while ( 0 != pthread_mutex_lock ( &recv_buf_mutex ) );
	recv_buf->Push ( tmpbuf  );
	cout << recv_buf->size () << " ";
	while ( 0 != pthread_mutex_unlock ( &recv_buf_mutex ) );
	cout << " done." << endl;

	while ( 0 != pthread_mutex_lock ( &recv_buf_mutex ) );
	frame_buf ptmpbuf = recv_buf->Pop ();
	while ( 0 != pthread_mutex_unlock ( &recv_buf_mutex ) );

	cout <<"size: " << ptmpbuf.size << endl;
	fwrite ( &(ptmpbuf.p_In_Frame), ptmpbuf.size, 1, pf );

	cout << endl;
	*/
}

void Consumer::onTimeout(const ptr_lib::shared_ptr<const Interest>& interest)
{
	std::cout<<"onTimeout:"<<std::endl;
	++callbackCount_;
	cout << "Time out for interest " << interest->getName().toUri() << endl;
}



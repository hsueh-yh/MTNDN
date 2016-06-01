#include "Consumer.h"


Consumer::Consumer () :
	recv_buf ( new CirQueue<frame_buf> ( 20 ) ),
	callbackCount_ ( 0 )
{
	pthread_mutex_init ( &recv_buf_mutex, NULL );
	
	
	pf = fopen ( "consumer.264", "wb+" );
	if ( pf == NULL )
	{
		cout << "open consumer.264 error" << endl;
		return;
	}
}

Consumer::~Consumer ()
{
	while ( 0 != pthread_mutex_destroy ( &recv_buf_mutex ));
	fclose ( pf );
}


void Consumer::onData(const ptr_lib::shared_ptr<const Interest>& interest, const ptr_lib::shared_ptr<Data>& data)
{
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
}

void Consumer::onTimeout(const ptr_lib::shared_ptr<const Interest>& interest)
{
	++callbackCount_;
	cout << "Time out for interest " << interest->getName().toUri() << endl;
}

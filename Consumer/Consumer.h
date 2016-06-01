#include <pthread.h>
#include <ndn-cpp/face.hpp>
#include "util/CircularQueue.h"

using namespace std;
using namespace ndn;
using namespace ndn::func_lib;


#define WIDTH 640
#define HEIGHT 480


struct frame_buf
{
	size_t size;
	uint8_t p_In_Frame[WIDTH * HEIGHT * 3 / 2];
};


class Consumer
{
public:
	Consumer ();


	~Consumer ();


	void onData(const ptr_lib::shared_ptr<const Interest>& interest, const ptr_lib::shared_ptr<Data>& data);
	

	void onTimeout(const ptr_lib::shared_ptr<const Interest>& interest);
	

	int callbackCount_;


private:

	FILE *pf;
	
	CirQueue<frame_buf> *recv_buf;
	pthread_mutex_t recv_buf_mutex;
};


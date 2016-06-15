#include <pthread.h>
#include <ndn-cpp/face.hpp>
#include <boost/shared_ptr.hpp>

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <thread>

#include "util/CircularQueue.h"
#include "utils.h"
#include "object.h"
#include "face-wrapper.h"
#include "frame-buffer.h"
#include "pipeliner.h"

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

	void init();

	void start();

	void onData(const ptr_lib::shared_ptr<const Interest>& interest,
				const ptr_lib::shared_ptr<Data>& data);

	void onTimeout(const ptr_lib::shared_ptr<const Interest>& interest);
	

	int callbackCount_;

	boost::shared_ptr<FaceWrapper> faceWrapper_;
	boost::shared_ptr<FrameBuffer> frameBuffer_;
	boost::shared_ptr<Pipeliner> pipeliner_;

private:

	FILE *pf;
	


	//CirQueue<frame_buf> *recv_buf;
	//pthread_mutex_t recv_buf_mutex;
};


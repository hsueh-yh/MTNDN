#include <ndn-cpp/face.hpp>

using namespace std;
using namespace ndn;
using namespace ndn::func_lib;

class Consumer
{
public:
	Consumer();


	void onData(const ptr_lib::shared_ptr<const Interest>& interest, const ptr_lib::shared_ptr<Data>& data);
	

	void onTimeout(const ptr_lib::shared_ptr<const Interest>& interest);
	

	int callbackCount_;
};
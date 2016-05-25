
#include <ndn-cpp/face.hpp>
#include <ndn-cpp/security/key-chain.hpp>

using namespace ndn;
using namespace std;

class Publisher {
public:
	Publisher(KeyChain &keyChain, const Name& certificateName)
		: keyChain_(keyChain), certificateName_(certificateName), responseCount_(0)
	{
	}

	// onInterest.
	void operator()
					(const ptr_lib::shared_ptr<const Name>& prefix,
					const ptr_lib::shared_ptr<const Interest>& interest, Face& face,
					uint64_t interestFilterId,
					const ptr_lib::shared_ptr<const InterestFilter>& filter);

	// onRegisterFailed.
	void operator()(const ptr_lib::shared_ptr<const Name>& prefix);


	int start();
	int stop();

	KeyChain keyChain_;
	//Name myPrefix;
	Name certificateName_;
	int responseCount_;

	int stat;
};

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>

#include "publisher.h"

#define HOST_DEFAULT "localhost"
#define PORT_DEFAULT 6363


using namespace std;


static boost::asio::io_service libIoService;
//static boost::shared_ptr<FaceWrapper> facewrapper;


boost::shared_ptr<FaceWrapper>
createFace(const std::string host, const int port)
{
    /*
    // if the uri did not specify a port
    if(std::string::npos == uri.find(":"))
        uri.append(":6363");

    //check ip format (not useful)


    set<std::string>::iterator iter;
    iter = hostSet_.find(uri);
    // the face already exists
    if(iter != hostSet_.end())
        return;

    hostSet_.insert(uri);
    */

#ifdef __SHOW_CONSOLE_
    std::cout << "Create Face: " << host<< ":"<<port << endl;
#endif

    NdnRtcUtils::setIoService(libIoService);

    //NdnRtcUtils::performOnBackgroundThread([=]()->void{
            NdnRtcUtils::createLibFace(host, port);
    //});
    NdnRtcUtils::startBackgroundThread();

    // Counter holds data used by the callbacks.
    //Consumer consumer(NdnRtcUtils::getLibFace()->getFaceWrapper());
    //facewrapper = NdnRtcUtils::getLibFace()->getFaceWrapper();
    return NdnRtcUtils::getLibFace()->getFaceWrapper();
}

int main(int argc, char** argv)
{
	try {
		// The default Face will connect using a Unix socket, or to "localhost".
		//Face face;

		// Use the system default key chain and certificate name to sign commands.
		//KeyChain keyChain;
		//face.setCommandSigningInfo(keyChain, keyChain.getDefaultCertificateName());

		// Also use the default certificate name to sign data packets.
		//Publisher publisher(keyChain, keyChain.getDefaultCertificateName());

		//boost::shared_ptr<FaceWrapper> facewrapper = createFace("10.103.240.100"，6363);
		boost::shared_ptr<FaceWrapper> facewrapper;
		facewrapper = createFace(HOST_DEFAULT, PORT_DEFAULT);
		//if (facewrapper)

		Publisher *publisher = new Publisher(facewrapper, "/video");
		if( !publisher->init() )
		{
			cout << "Publisher init fail" << endl;
			return 0;
		}

//		Name prefix("/video");
//		cout << "Register prefix  " << prefix.toUri() << endl;
		// TODO: After we remove the registerPrefix with the deprecated OnInterest,
		// we can remove the explicit cast to OnInterestCallback (needed for boost).
		//face.registerPrefix(prefix, (const OnInterestCallback&)func_lib::ref(publisher), func_lib::ref(publisher));
		

		publisher->start();
		// The main event loop.
		// Wait forever to receive one interest for the prefix.
//		while ( publisher->stat ) {
//			face.processEvents();
//			// We need to sleep for a few milliseconds so we don't use 100% of the CPU.
//			usleep(100);
//		}
	}
	catch (std::exception& e) {
		cout << "exception: " << e.what() << endl;
	}
	return 0;
}

//
//  ndnrtc-object.cpp
//  ndnrtc
//
//  Copyright 2013 Regents of the University of California
//  For licensing details see the LICENSE file.
//
//  Author:  Peter Gusev
//  Created: 8/21/13
//

#include <stdarg.h>
#include <thread>
#include <mutex>
#include <chrono>


#include "object.h"
#include "utils.h"

using namespace std;
using namespace ndn;

//******************************************************************************
/**
 * @name NdnRtcComponent class
 */
NdnRtcComponent::NdnRtcComponent():
watchdogTimer_(NdnRtcUtils::getIoService()),
isJobScheduled_(false),
isTimerCancelled_(false)
{}

NdnRtcComponent::NdnRtcComponent(INdnRtcComponentCallback *callback):
callback_(callback),
watchdogTimer_(NdnRtcUtils::getIoService()),
isJobScheduled_(false),
isTimerCancelled_(false)
{}

NdnRtcComponent::~NdnRtcComponent()
{
    try {
        stopJob();
    }
    catch (...) {
    }
}

void NdnRtcComponent::onError(const char *errorMessage, const int errorCode)
{
    if (hasCallback())
    {
        ptr_lib::lock_guard<ptr_lib::mutex> scopedLock(callbackMutex_);
        callback_->onError(errorMessage, errorCode);
    }
    else
    {
        std::cout << "error" << std::endl;
        //LogErrorC << "error occurred: " << string(errorMessage) << endl;
        //if (logger_) logger_->flush();
    }
}
/*
int NdnRtcComponent::notifyError(const int ecode, const char *format, ...)
{
    va_list args;
    
    static char emsg[256];
    
    va_start(args, format);
    vsprintf(emsg, format, args);
    va_end(args);
    
	if ( hasCallback () )
	{
		callback_->onError ( emsg, ecode );
	}
	else
		cout << "error" << endl;
        //LogErrorC
       // << "error (" << ecode << ") occurred: "
       // << string(emsg) << endl;
    
    return ecode;
}
*/
//std::string
//NdnRtcComponent::getDescription() const
//{
//    if (description_ == "")
//    {
//        std::stringstream ss;
//        ss << "NdnRtcObject "<< std::hex << this;
//        return ss.str();
//    }
//    
//    return description_;
//}


//ptr_lib::thread
//NdnRtcComponent::startThread(ptr_lib::function<bool ()> threadFunc)
//{
//    ptr_lib::thread threadObject = ptr_lib::thread([threadFunc](){
//        bool result = false;
//        do {
//            try
//            {
//                ptr_lib::this_thread::interruption_point();
//                {
//                    ptr_lib::this_thread::disable_interruption di;
//                    result = threadFunc();
//                }
//            }
//            catch (thread_interrupted &interruption)
//            {
//                result = false;
//            }
//        } while (result);
//    });
    
//    return threadObject;
//}

//void
//NdnRtcComponent::stopThread(thread &threadObject)
//{
//    threadObject.interrupt();
    
//    if (threadObject.joinable())
//    {
//        bool res = threadObject.try_join_for(boost::chrono::milliseconds(500));
                                             
//        if (!res)
//            threadObject.detach();
//    }
//}


void NdnRtcComponent::scheduleJob(const unsigned int usecInterval,
                                  std::function<bool()> jobCallback)
{
    std::lock_guard<std::recursive_mutex> scopedLock(this->jobMutex_);
    
    watchdogTimer_.expires_from_now(std::chrono::microseconds(usecInterval));
    //watchdogTimer_.expires_from_now(boost::posix_time::milliseconds(usecInterval));
    //watchdogTimer_.expires_from_now(boost::chrono::microseconds(usecInterval));
    isJobScheduled_ = true;
    isTimerCancelled_ = false;
    
    watchdogTimer_.async_wait([this, usecInterval, jobCallback](const boost::system::error_code& code){
        if (code != boost::asio::error::operation_aborted)
        {
            if (!isTimerCancelled_)
            {
                isJobScheduled_ = false;
                std::lock_guard<std::recursive_mutex> scopedLock(this->jobMutex_);
                bool res = jobCallback();
                if (res)
                    this->scheduleJob(usecInterval, jobCallback);
            }
        }
    });

}


void NdnRtcComponent::stopJob()
{
    NdnRtcUtils::performOnBackgroundThread([this]()->void{
        std::lock_guard<std::recursive_mutex> scopedLock(jobMutex_);
        watchdogTimer_.cancel();
        isTimerCancelled_ = true;
    });
    
    isJobScheduled_ = false;
}


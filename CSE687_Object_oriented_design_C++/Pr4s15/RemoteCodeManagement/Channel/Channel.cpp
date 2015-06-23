/////////////////////////////////////////////////////////////////////////////
// Channel.cpp   - communication channel for project #4                    //
//            -  build as DLL then use this DLL to use native functions for// 
//              communication purpose.                                     //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project4S15                //
// Modified by: Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
// Author :     Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "Channel.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Client1/Client1.h"

#include <string>
#include <thread>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// Channel class
// - send and reads messages from Client.
//
class Channel : public IChannel
{
public:
	Channel(std::string ip, int port);
	void start();
	void stop();
	void postMessage(const Message& msg) { client_.postMessage(msg); }
	Message getMessage() { return client_.getMessage(); }
private:
	std::thread thread_;
	Client1 client_;
	bool stop_ = false;
	
};

//----< pass ip and port to client >-------------------------------
Channel::Channel(std::string ip, int port) : client_(ip, port) {}

void Channel::start()
{
	std::cout << "\n  Communication Channel starting up";
	client_.start();
}
//----< signal server thread to stop >---------------------------------------

void Channel::stop() 
{
	//client_.stop();
}

//----< create communication channel >---------------------------------------
IChannel* ObjectFactory::createChannel(std::string ip, int port)
{
	return new Channel(ip, port);
}

#ifdef TEST_CHANNEL

//----< test stub >----------------------------------------------------------
int main()
{
	ObjectFactory objFact;
	IChannel* pMockChannel = objFact.createChannel("localhost", 9080);
	pMockChannel->start();
	Message msg;
	pMockChannel->postMessage(msg);
	std::cin.get();
}
#endif

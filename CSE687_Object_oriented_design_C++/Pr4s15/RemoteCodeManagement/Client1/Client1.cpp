/////////////////////////////////////////////////////////////////////////////
// Client1.cpp - Send file to server and receive ACK from server           //
//																		   //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project4S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "Client1.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Display/Display.h"

//----< conect to particular socket as per provided input of ip address and port number. >---------------

bool Client1::connectSocket(SocketConnecter& si, std::string ipAddress, int port)
{
	while(!si.connect(ipAddress, port))
	{
		Display::show("client1 waiting to connect");
		::Sleep(100);
	}
	Display::show("client1 connected on port number : " + std::to_string(port));
	return true;
}

//----< sender function which will process request which has been enqueue >---------------

void Client1::processRequest(SocketConnecter& si, Message request)
{
	Sender sender;
	connectSocket(si, request.destAdd, request.destPort);
	request.senderPort = port_;
	request.sourceAddress = address_;
	if(request.commandType == UPLOAD)
		sender.sendFiledata(si, request, true);
	else if(request.commandType == GETFILES)
		sender.getsFiles(si,request);
	else if(request.commandType == DOWNLOAD)
		sender.sendDownloadRequest(si, request);
	else if(request.commandType == TEXTSEARCH)
		sender.sendTextSearchRequest(si, request);
	si.shutDown();
}

//----< here take request from blocking queue and process that request. >---------------

void Client1::startSender()
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;

		while(true)
		{
			auto request = sendingQueue.deQ();
			processRequest(si, request);

		}
	}
	catch(std::exception& ex)
	{
		Display::show("  Exception caught:");
		Display::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< start receiver and initialize socket on particular port. >---------------

void Client1::startReceiver()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(port_, Socket::IP6);
		sl.start(client1Receiver);

		Display::show("press key to exit: ");
		std::cout.flush();
		std::cin.get();
	}
	catch(std::exception& ex)
	{
		Display::show("  Exception caught:", true);
		Display::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< start sender thread. >---------------

void Client1::startSenderThread()
{
	std::thread clientThread(&Client1::startSender, this);
	clientThread.detach();
}

//----< start receiver thread. >---------------

void Client1::startReceiverThread()
{
	std::thread serverThread(&Client1::startReceiver, this);
	serverThread.detach();
}

//----< post message from WPF application >---------------

void Client1::postMessage(const Message& msg)
{
	sendingQueue.enQ(msg);
}

//----< WPF application will call this method to get message from receving queue. >---------------

Message Client1::getMessage()
{
	Message msg;
	std::string result = rcvQueue.deQ();
	msg = msg.parseMessage(result);
	//std::cout << msg.body << std::endl;
	return msg;
}

//----<process command line arguments >---------------
bool Client1::processCommandArgs(int argc, char** argv)
{
	Message msg;
	if(argc > 1 && argv[1])
	{
		std::string sourceDetails = argv[1];
		std::vector<std::string> source = msg.split(sourceDetails, ':');
		if(source.size() == 2)
		{
			address_ = source[0];
			port_ = stoi(source[1]);
		}
	}
	if(argc > 2 && (std::string(argv[2]) == "/s" || std::string(argv[2]) == "/f"))
	{
		if(std::string(argv[2]) == "/s")
			isFileTest_ = false;
		else if(std::string(argv[2]) == "/f")
			isFileTest_ = true;
		for(int i = 3; i < argc; i++)
			commandInformation_.push_back(argv[i]);
	}
	else
		return false;
	return true;
}

//----<start sender and receiver>---------------
void Client1::startTask()
{
	Message msg;
	startReceiverThread();

	std::string message;
	if(commandInformation_.size() > 0)
	{
		bool flag = false;
		for(size_t i = 0; i < commandInformation_.size(); i++)
		{
			std::vector<std::string> details = msg.split(commandInformation_[i], '@');
			if(details.size() == 2)
			{
				std::string destDetails = details[0];
				std::vector<std::string> dest = msg.split(destDetails, ':');
				if(dest.size() == 2)
				{
					if(!flag)
					{
						message = msg.createOperation(TEST_MESSAGE, "NA", dest[0], stoi(dest[1]), "This is test Http message to " + destDetails + " This message is from client1");
						flag = true;

						sendingQueue.enQ(msg);
					}
					if(isFileTest_)
					{
						message = msg.createOperation(UPLOAD, details[1], dest[0]
													  , stoi(dest[1]));
					}
					else
					{
						message = msg.createOperation(SEND_STRING, "NA", dest[0], stoi(dest[1]), "Hello how are you doing, " + dest[0] + "@" + dest[1] + " This message is from client1");
					}
					
					sendingQueue.enQ(msg);
				}
			}
		}
	}
	startSenderThread();
}

#ifdef TEST_CLIENT1
int main(int argc, char* argv[])
{
	Display::show(" *** Client1 ***  ", true);
	Display::show("\n", true);
	Client1 client1;
	if(client1.processCommandArgs(argc, argv))
	{
		client1.startTask();
	}
	else
	{
		Display::show("\n\n Please provide correct command line arguments, Please check README file for more details.\n\n");
	}
	return 0;
}
#endif
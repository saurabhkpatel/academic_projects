/////////////////////////////////////////////////////////////////////////////
// Server1.cpp - Send file to server and receive ACK from server           //
//																		   //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project3S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "Server1.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Display/Display.h"

//----< wrapper function to enqueue message in blocking queue >---------------

void Server1::enQ(std::string data)
{
	queue.enQ(data);
}

//----< wrapper function to dequeue message from blocking queue >---------------

std::string Server1::deQ()
{
	return queue.deQ();
}

//----< conect to particular socket as per provided input of ip address and port number. >---------------

bool Server1::connectSocket(SocketConnecter& si, std::string ipAddress, int port)
{
	while(!si.connect(ipAddress, port))
	{
		Display::show(" Server1's sender waiting to connect",true);
		::Sleep(100);
	}
	Display::show(" Server1's sender connected : " + ipAddress + ":" + std::to_string(port), true);
	return true;
}

//----< sender function which will process request which has been enqueue >---------------

void Server1::processRequest(SocketConnecter& si, std::string request)
{
	Sender sender;

	Message rcvMessage;
	rcvMessage = rcvMessage.parseMessage(request);

	if(rcvMessage.commandType == UPLOAD_ACK)
	{
		if(connectSocket(si, rcvMessage.destAdd, rcvMessage.destPort))
		{
			Display::show(" File " + rcvMessage.fileName + "'s Acknowledgement is sent to : " + rcvMessage.destAdd + ":" + std::to_string(rcvMessage.destPort), true);
			si.sendString(request);
		}
	}
	else if((rcvMessage.commandType == TEXTSEARCH_RESPONSE))
	{
		if(connectSocket(si, rcvMessage.destAdd, rcvMessage.destPort))
		{
			si.sendString(request);
		}
	}
	else if((rcvMessage.commandType == GETFILES_RESPONSE))
	{
		if(connectSocket(si, rcvMessage.destAdd, rcvMessage.destPort))
		{
			si.sendString(request);
		}
	}
	else if(rcvMessage.commandType == DOWNLOAD_REQUEST)
	{
		if(connectSocket(si, rcvMessage.destAdd, rcvMessage.destPort))
		{
			sender.sendFiledata(si, rcvMessage, false);
		}
	}
}

//----< here take request from blocking queue and process that request. >---------------

void Server1::startSender()
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while(true)
		{
			auto request = queue.deQ();
			processRequest(si, request);
		}
	}
	catch(std::exception& ex)
	{
		Display::show("  Exception caught:", always);
		Display::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< start receiver and initialize socket on particular port. >---------------

void Server1::startReceiver()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(port_, Socket::IP6);
		
		sl.start(server1Receiver);

		Display::show("press key to exit: ", always);
		std::cout.flush();
		std::cin.get();
	}
	catch(std::exception& ex)
	{
		Display::show("  Exception caught:", always);
		Display::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< start sender thread. >---------------

void Server1::startSenderThread()
{
	std::thread clientThread(&Server1::startSender, this);
	clientThread.join();
}

//----< start receiver thread. >---------------

void Server1::startReceiverThread()
{
	std::thread receiverThread(&Server1::startReceiver, this);
	receiverThread.detach();
}

//----<process command line arguments >---------------
bool Server1::processCommandArgs(int argc, char** argv)
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
			Display::show("  Address :  "+address_,true) ;
			Display::show("  Port    :  " +std::to_string(port_),true);
			return true;
		}
		return false;
	}
	return false;
}


#ifdef TEST_SERVER1
int main(int argc, char* argv[])
{
	Display::show("  ***  Server1  ***  ",true);
	Display::show("\n", true);
	//Display::demonstrateMessage();
	//Display::show("\n", true);
	//Display::title(" All received files will be stored at ./receive/ directory.", '*');
	Server1 server1;
	server1.processCommandArgs(argc, argv);
	server1.startReceiverThread();
	server1.startSenderThread();
}
#endif
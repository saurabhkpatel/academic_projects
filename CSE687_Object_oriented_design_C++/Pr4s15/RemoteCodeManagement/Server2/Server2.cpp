/////////////////////////////////////////////////////////////////////////////
// Server2.cpp - Send file to server and receive ACK from server           //
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
#include "Server2.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Display/Display.h"

//----< wrapper function to enqueue message in blocking queue >---------------

void Server2::enQ(std::string data)
{
	queue.enQ(data);
}

//----< wrapper function to dequeue message from blocking queue >---------------

std::string Server2::deQ()
{
	return queue.deQ();
}


//----< conect to particular socket as per provided input of ip address and port number. >---------------

bool Server2::connectSocket(SocketConnecter& si, std::string ipAddress, int port)
{
	while(!si.connect(ipAddress, port))
	{
		Display::show(" Server2's sender waiting to connect",true);
		::Sleep(100);
	}
	Display::show(" Server2's sender connected : " + ipAddress + ":" + std::to_string(port),true);
	return true;
}

//----< sender function which will process request which has been enqueue >---------------
void Server2::processRequest(SocketConnecter& si, std::string request)
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

void Server2::startSender()
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
		Display::show("  Exception caught:", true);
		Display::show(std::string("\n  ") + ex.what() + "\n\n",true);
	}
}

//----< start receiver and initialize socket on particular port. >---------------

void Server2::startReceiver()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(port_, Socket::IP6);
		sl.start(server2Receiver);

		//Display::show("press key to exit: ", true);
		std::cout.flush();
		std::cin.get();
	}
	catch(std::exception& ex)
	{
		Display::show("  Exception caught:", true);
		Display::show(std::string("\n  ") + ex.what() + "\n\n",true);
	}
}

//----< start sender thread. >---------------

void Server2::startSenderThread()
{
	std::thread senderThread(&Server2::startSender, this);
	senderThread.join();
}

//----< start receiver thread. >---------------

void Server2::startReceiverThread()
{
	std::thread receiverThread(&Server2::startReceiver, this);
	receiverThread.detach();
}


//----<process command line arguments >---------------
bool Server2::processCommandArgs(int argc, char** argv)
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
			Display::show("  Address :  " + address_,true);
			Display::show("  Port    :  " + std::to_string(port_), true);
			return true;
		}
		return false;
	}
	return false;
}


#ifdef TEST_SERVER2
int main(int argc, char* argv[])
{
	Display::show("  ***  Server2 ***  ",true);
	//Display::show("\n", true);
	//Display::demonstrateMessage();
	//Display::show("\n", true);
	//Display::title(" All received files will be stored at ./receive/ directory.", '*');
	Server2 server2;
	server2.processCommandArgs(argc, argv);
	server2.startReceiverThread();
	server2.startSenderThread();
}
#endif
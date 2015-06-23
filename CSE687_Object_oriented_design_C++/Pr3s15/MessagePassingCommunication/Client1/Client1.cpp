/////////////////////////////////////////////////////////////////////////////
// Client1.cpp - Send file to server and receive ACK from server           //
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
#include "Client1.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Display/Display.h"

//----< wrapper function to enqueue message in blocking queue >---------------

void Client1::enQ(std::string data)
{
	queue.enQ(data);
}

//----< wrapper function to dequeue message from blocking queue >---------------

std::string Client1::deQ()
{
	return queue.deQ();
}

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

void Client1::processRequest(SocketConnecter& si, std::string request)
{
	Message msg;
	Sender sender;
	std::vector<std::string> elements = msg.split(request, ETX);
	std::map<std::string, std::string> command = msg.createMap(elements);
	// parse operation and do.
	connectSocket(si, command[DESTINATION_ADD], std::stoi(command[DESTINATION_PORT]));
	if(command[COMMAND_TYPE] == UPLOAD)
	{
		sender.sendFiledata(si, command[FILE_NAME], address_, command[DESTINATION_ADD], port_, stoi(command[DESTINATION_PORT]));
	}
	else if(command[COMMAND_TYPE] == SEND_STRING)
	{
		sender.sendString(si, command[OPERATION_BODY], address_, command[DESTINATION_ADD], port_, stoi(command[DESTINATION_PORT]));
	}
	else if(command[COMMAND_TYPE] == SEND_BUFFER)
	{
		sender.sendBuffer(si, command[OPERATION_BODY], address_, command[DESTINATION_ADD], port_, stoi(command[DESTINATION_PORT]));
	}
	else if(command[COMMAND_TYPE] == TEST_MESSAGE)
	{
		sender.sendHttpMessage(si, command[OPERATION_BODY], address_, command[DESTINATION_ADD], port_, stoi(command[DESTINATION_PORT]));
	}
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
			auto request = queue.deQ();
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
		Receiver client1Receiver;
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
	std::thread clientThread(&Client1::startSender, Client1());
	clientThread.join();
}

//----< start receiver thread. >---------------

void Client1::startReceiverThread()
{
	std::thread serverThread(&Client1::startReceiver, Client1());
	serverThread.detach();
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
	if(argc > 2 && (std::string(argv[2]) == "/s" ||std::string( argv[2]) == "/f"))
	{
		if(std::string(argv[2]) == "/s")
			isFileTest_ = false;
		else if(std::string(argv[2]) == "/f")
			isFileTest_ = true;
		for(int i =3; i < argc; i++)
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
						enQ(message);
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
					enQ(message);
				}
			}
		}
	}
	startSenderThread();
}

#ifdef TEST_CLIENT1
int main(int argc, char* argv[])
{
	Display::title(" *** Client1 ***  ",'=');
	Display::show("\n", true);
	Display::demonstrateMessage();
	Client1 client1;
	if(client1.processCommandArgs(argc, argv))
	{
		client1.startTask();
	}
	else
	{
		Display::displayErrorLog("\n\n Please provide correct command line arguments, Please check README file for more details.\n\n");
	}
	return 0;
}
#endif
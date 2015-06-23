#ifndef CLIENT1_H 
#define CLIENT1_H

/////////////////////////////////////////////////////////////////////////////
// Client1.h - Send file to server and receive ACK from server             //
//																		   //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project3S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* ===============================
* This package demonstrate the server and client communication. First of all, Sender will do request
* to server for file upload and read file in blocks and send them to server. Once server wil done with
* fle upload message. Server will enqueue that message along with filename and sender ip and port information.
* Now, server's sender function will read this enqueue message from Blocking queue which is shared between Receiver
* and server. After dequeue it will send that message (File ACK) to desired ip and port.
*
* Public Interface:
* ==============================
* void startReceiverThread(); // start receiver thread.
* void startSender();         // start sender.
* void startSenderThread();   // start sender thread.
* void startReceiver();       // start receiver.
* bool connectSocket(SocketConnecter& si, std::string ipAddress, int port); // connect socket.
* void processRequest(SocketConnecter& si, std::string request); // deq from queue and process requet
* void enQ(std::string data); // enQ data in queue
* std::string deQ(); // retrive data from queue.
* bool processCommandArgs(int argc, char** argv); // process command line arguments and store in member variables
* void startTask(); // start client1's task
*
* Required Files:
* ============================
* Client1.h, Client1.cpp, HttpMessage.cpp , HttpMessage.h
* Sender.cpp, Sender.h, Reciever.cpp, Reciever.h
* Sockets.cpp , Sockets.h, AppHelpers.cpp AppHealpers.h
* Display.h
* 
*
* Build Command:
* ============================
* devenv RemoteCodeManagement.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 2.0 : 28 April 15
* - Integration with Project #4
* ver 1.0 : 10 April 15
* - first release
*/

#include <string>
#include <iostream>
#include "../HttpMessage/HttpMessage.h"
#include "../sockets/sockets.h"
#include "../Receiver/Receiver.h"

class Client1
{
public:
	Client1(std::string address = "localhost", int port = 8080) : address_(address), port_(port), client1Receiver(rcvQueue)
	{
	}
	void startReceiverThread();
	void startSender();
	void startSenderThread();
	void startReceiver();
	bool connectSocket(SocketConnecter& si, std::string ipAddress, int port);
	bool processCommandArgs(int argc, char** argv);
	void startTask();
	void postMessage(const Message& msg);
	void processRequest(SocketConnecter& si, Message request);
	Message getMessage();
	void start()
	{
		startReceiverThread();
		startSenderThread();
	}
private:
	BlockingQueue<Message> sendingQueue;// = QServer<std::string, 1>().get();
	BlockingQueue<std::string> rcvQueue;// = QServer<std::string, 50>().get();
	std::string address_;
	int port_;
	bool isFileTest_;
	std::vector<std::string> commandInformation_;
	Receiver client1Receiver;
};

#endif


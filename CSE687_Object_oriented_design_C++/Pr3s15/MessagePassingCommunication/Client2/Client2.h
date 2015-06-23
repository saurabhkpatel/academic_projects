#ifndef CLIENT2_H 
#define CLIENT2_H

/////////////////////////////////////////////////////////////////////////////
// Client2.h - Send file to server and receive ACK from server             //
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
* bool processCommandArgs(int argc, char** argv); // process command line arguments.
* void startTask(); // start task of Client2 that means it will start sender and receiver thread.
*
* Required Files:
* ============================
* Client2.h, Client2.cpp, HttpMessage.cpp , HttpMessage.h
* Sender.cpp, Sender.h, Reciever.cpp, Reciever.h
* Sockets.cpp , Sockets.h, AppHelpers.cpp AppHealpers.h
*
*
* Build Command:
* ============================
* devenv Pr3s15.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 1.0 : 9 March 15
* - first release
*/

#include <string>
#include <iostream>
#include "../QServer/QServer/QServer.h"
#include "../sockets/sockets.h"

class Client2
{
public:
	Client2(std::string address = "localhost", int port = 8081) : address_(address), port_(port) {}
	void startReceiverThread();
	void startSender();
	void startSenderThread();
	void startReceiver();
	bool connectSocket(SocketConnecter& si, std::string ipAddress, int port);
	void processRequest(SocketConnecter& si, std::string request);
	void enQ(std::string data);
	std::string deQ();
	bool processCommandArgs(int argc, char** argv);
	void startTask();
private:
	BlockingQueue<std::string> &queue = QServer<std::string, 2>().get();
	std::string address_;
	int port_;
	bool isFileTest_;
	std::vector<std::string> commandInformation_;
};

#endif

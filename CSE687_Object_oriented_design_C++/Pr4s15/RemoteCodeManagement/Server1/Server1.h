#ifndef SERVER1_H 
#define SERVER1_H

/////////////////////////////////////////////////////////////////////////////
// Server1.h - Send file to server and receive ACK from server             //
//																		   //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project4S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* ===============================
* This package demonstrate the server and client communication.Sever1 class starts its own sender
* and receiver. Server will start listening on some port. Once data or file will be received from Client1
* then server will add those messages in blocking queue. If there is file block request then it will
* directly write on file. Sever's sender will dequeue those messages and send to Client as per their ip
* and port numbers.
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
*
* Required Files:
* ============================
* Server1.h, Server1.cpp, HttpMessage.cpp , HttpMessage.h
* Sender.cpp, Sender.h, Reciever.cpp, Reciever.h
* Sockets.cpp , Sockets.h, AppHelpers.cpp AppHealpers.h
* DataStore.cpp DataStore.h FileManager.h, FileManager.cpp
* Filesystem.h FileSystem.cpp SerachText.cpp SearchText.h
*
* Build Command:
* ============================
* devenv RemoteCodeManagement.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 2.0 : 28 April 15
* - second release
* - Support Project 4 related changes.
* ver 1.0 : 9 April 15
* - first release
*/


#include <string>
#include <iostream>
#include "../sockets/sockets.h"
#include "../Receiver/Receiver.h"

class Server1
{
public:
	Server1(std::string address = "localhost", int port = 9085) : address_(address), port_(port), server1Receiver(queue,"./server1root/") {}
	void startReceiverThread();
	void startSender();
	void startSenderThread();
	void startReceiver();
	bool connectSocket(SocketConnecter& si, std::string ipAddress, int port);
	void processRequest(SocketConnecter& si, std::string request);
	void enQ(std::string data);
	std::string deQ();
	bool processCommandArgs(int argc, char** argv);
private:
	BlockingQueue<std::string> queue;
	std::string address_;
	int port_;
	Receiver server1Receiver;
};
#endif


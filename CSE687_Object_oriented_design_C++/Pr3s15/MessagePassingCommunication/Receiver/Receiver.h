#ifndef RECEIVER_H
#define RECEIVER_H

/////////////////////////////////////////////////////////////////////////////
// Receiver.h - This package handles operations related to do once it      //
//              recevied any request from client.                          //
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
* This package provides operations to do once server receives any request from client.
* such that file write if file upload request come, send string message if any ACK request come.
*
* Public Interface:
* ==============================
* void operator()(Socket& socket_); // Functor Call.
* bool fileUpload(Socket& socket_, std::map<std::string, std::string> headerData); // Fileupload operation, once file upload request came
* bool testStringHandling(Socket& socket_); // test function to test string buffer handling.
* bool testBufferHandling(Socket& socket); // test function to test buffer handling.
* void fileUploadHandler(Socket& socket_,std::map<std::string, std::string> headerData); // Handle file upload task.
* void stringReceiveHandler(std::map<std::string, std::string> headerData, std::string recvString);// string handler
* bool bufferReading(Socket& socket_, int size, Socket::byte* buffer); //  function to read buffer bytes from socket.
*
* Required Files:
* ============================
* Receiver.cpp, Receiver.h, HttpMessage.cpp , HttpMessage.h
* Sockets.cpp , Sockets.h, AppHelpers.cpp AppHealpers.h,
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

#include <iostream>
#include <string>
#include <map>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <mutex>
#include "../sockets/sockets.h"
#include "../QServer/QServer/QServer.h"
#include "../ApplicationHelpers/AppHelpers.h"


class Receiver
{
public:
	void operator()(Socket& socket_);
	bool fileUpload(Socket& socket_, std::map<std::string, std::string> headerData);
	bool testStringHandling(Socket& socket_);
	bool testBufferHandling(Socket& socket);
	bool bufferReading(Socket& socket_, int size, Socket::byte* buffer);
	void fileUploadHandler(Socket& socket_, std::map<std::string, std::string> headerData);
	void stringReceiveHandler(std::map<std::string, std::string> headerData, std::string recvString);
private:
	BlockingQueue<std::string> &qserver = QServer<std::string, 50>().get();
};

#endif
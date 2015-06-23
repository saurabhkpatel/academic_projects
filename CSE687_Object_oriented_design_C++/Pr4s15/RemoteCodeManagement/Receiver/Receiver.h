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
* void getServerFiles(Message& rcvmessage); // Get Server root file list, here client will request catagory type in message class.
* void fileDownloadHandler(Socket& socket_, Message& rcvmessage); // File download handler 
* void textSearchHandler(Message& rcvmessage); // text search from files.
*
* Required Files:
* ============================
* Receiver.cpp, Receiver.h, HttpMessage.cpp , HttpMessage.h
* Sockets.cpp , Sockets.h, AppHelpers.cpp AppHealpers.h,
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

#include <iostream>
#include <string>
#include <map>
#include <iostream>
#include <stdlib.h>
#include "../sockets/sockets.h"
#include "../HttpMessage/HttpMessage.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

#define CLIENTROOTPATH "./download/"

class Receiver
{
public:
	Receiver(BlockingQueue<std::string> &arg, std::string rcvPath = "") : qserver(arg), rcvPath_(rcvPath){}
	void operator()(Socket& socket_);
	bool fileUpload(Socket& socket_, Message& rcvmessage);
	bool fileWrite(Socket& socket, Message& rcvmessage, std::string filePath);
	bool testStringHandling(Socket& socket_);
	bool testBufferHandling(Socket& socket);
	bool bufferReading(Socket& socket_, int size, Socket::byte* buffer);
	void fileUploadHandler(Socket& socket_, Message& rcvMessage);
	void stringReceiveHandler(std::map<std::string, std::string> headerData, std::string recvString);
	void getServerFiles(Message& rcvmessage);
	void fileDownloadHandler(Socket& socket_, Message& rcvmessage);
	void textSearchHandler(Message& rcvmessage);
private:
	BlockingQueue<std::string> &qserver;
	std::string rcvPath_;
};

#endif
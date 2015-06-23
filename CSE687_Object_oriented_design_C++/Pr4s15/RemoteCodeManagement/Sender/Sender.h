#ifndef SENDER_H
#define SENDER_H

/////////////////////////////////////////////////////////////////////////////
// Sender.h - This file provides functions to send different types of data //
//            to server.                                                   //
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
* This package provides basically functions or operations which are related to sent some data to Server.
* Here, it includes operations which send file upload, send test string data or send any other request from
* client to server.
*
* Public Interface:
* ==============================
* void testStringHandling(Socket& si); // test function to communication string between server and receiver.
* void testBufferHandling(Socket& si); // test function to communication buffer bytes between server and receiver.
* void getFiles(Socket& si); // get list of uploaded files from server.
* void getsFiles(Socket& si, Message msg); // Get files or search files.
* void sendDownloadRequest(Socket& si, Message msg);
* void sendTextSearchRequest(Socket& si, Message msg);
* void sendFiledata(Socket& si, Message requestMessage, bool isUpload);
* void sendHttpMessage(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort); // sent http message to server and display.
*
* Required Files:
* ============================
* Sender.cpp, Sender.h, HttpMessage.cpp , HttpMessage.h
* Sockets.cpp , Sockets.h, AppHelpers.cpp AppHealpers.h,
* 
* Build Command:
* ============================
* devenv RemoteCodeManagement.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 1.0 : 28 April 15
* ver 2.0 : 28 April 15
* - second release
* - Support Project 4 related changes.
* ver 1.0 : 9 April 15
* - first release
*/

#include <iostream>
#include <fstream>
#include "../sockets/sockets.h"
#include "../HttpMessage/HttpMessage.h"

class Sender
{
public:
	void getsFiles(Socket& si, Message msg);
	void sendDownloadRequest(Socket& si, Message msg);
	void sendTextSearchRequest(Socket& si, Message msg);
	void sendFiledata(Socket& si, Message requestMessage, bool isUpload);
	void sendString(Socket& si, std::string fileName, std::string senderIP, std::string destIP, int senderPort, int destPort);
	void sendBuffer(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort);
	void sendHttpMessage(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort);
};

#endif
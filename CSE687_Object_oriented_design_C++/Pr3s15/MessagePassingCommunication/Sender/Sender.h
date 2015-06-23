#ifndef SENDER_H
#define SENDER_H

/////////////////////////////////////////////////////////////////////////////
// Sender.h - This file provides functions to send different types of data //
//            to server.                                                   //
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
* This package provides basically functions or operations which are related to sent some data to Server.
* Here, it includes operations which send file upload, send test string data or send any other request from
* client to server.
*
* Public Interface:
* ==============================
* void testStringHandling(Socket& si); // test function to communication string between server and receiver.
* void testBufferHandling(Socket& si); // test function to communication buffer bytes between server and receiver.
* void getFiles(Socket& si); // get list of uploaded files from server.
* void sendFiledata(Socket& si, std::string fileName, std::string senderIP, std::string destIP, int senderPort, int destPort); // upload file on server side. This function will send header for each file block data.
* void sendHttpMessage(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort); // sent http message to server and display.
*
* Required Files:
* ============================
* Sender.cpp, Sender.h, HttpMessage.cpp , HttpMessage.h
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
#include <fstream>
#include "../sockets/sockets.h"

class Sender
{
public:
	void getFiles(Socket& si);
	void sendFiledata(Socket& si, std::string fileName, std::string senderIP, std::string destIP, int senderPort, int destPort);
	void sendString(Socket& si, std::string fileName, std::string senderIP, std::string destIP, int senderPort, int destPort);
	void sendBuffer(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort);
	void sendHttpMessage(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort);
};

#endif
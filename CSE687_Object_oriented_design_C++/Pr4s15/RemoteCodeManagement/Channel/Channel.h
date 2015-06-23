#ifndef CHANNEL_H
#define CHANNEL_H

/////////////////////////////////////////////////////////////////////////////
// Channel.h   - communication channel for project #4                      //
//            -  build as DLL then use this DLL to use native functions for// 
//              communication purpose.                                     //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project4S15                //
// Modified by: Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
// Author :     Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides interface to Windows WPF application, using this interface
*  WPF application can use below object factory function and initilaizes channel object
*  so WPF application can use that object for further query.
*
* Public Interface:
* ==============================
* 	virtual Message getMessage() = 0; // get message from client receving queue.
* 	virtual void postMessage(const Message& msg) = 0; // post message to client's sending queue.
* 	virtual void start() = 0; //start client.
* 	virtual void stop() = 0; // stop client.
*  Required Files:
*  ---------------
*  Channel.h, channel.cpp, Client1.cpp, DataStore.cpp FileManager.cpp
*  Filesystem.cpp HttpMessage.cpp, Receiver.cpp SerachText.cpp, Sender.cpp, Sockets.cpp
*  Filesystem.h HttpMessage.h, Receiver.h SerachText.h, Sender.h, Sockets.h
*  DataStore.h, FileManager.h Client1.h
*
*  Build Command:
*  --------------
* devenv RemoteCodeManagement.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 28 Apr 2015
*      - First Release.
*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include "../HttpMessage/HttpMessage.h"

struct IChannel
{
public:
	virtual Message getMessage() = 0;
	virtual void postMessage(const Message& msg) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
};

extern "C" {
	struct ObjectFactory
	{
		DLL_DECL IChannel* createChannel(std::string ip, int port);
	};
}

#endif
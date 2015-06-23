#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h - Supports display functions.                                 //
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
* This package will provides functions to display the client and server communication related logs.
*
* Public Interface:
* ==============================
* static void title(const std::string& title, char ch = '=') //display title with underline.
* static void displaySendInformation(std::string str,std::string destIP, int destPort, bool isFile=true, bool isString=true) // display send file information.
* static void displayReceviedFile(std::string fileName) //display received file information
* static void displayErrorLog(std::string message) // display error log information.
* static void show(std::string message, bool isAlways=false) //	dsplay norml log message information
* static void displayHttpMessage(std::string message, std::map<std::string, std::string> headerData) // display http message.
*
* Required Files:
* ============================
*  Display.cpp, Display.h , AppHelpers.h , HttpMessage.h
*
* Build Command:
* ============================
* devenv Pr2s15.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 1.0 : 21 March 15
* - first release
*/
#include <iostream>
#include <string>
#include <map>
#include "../ApplicationHelpers/AppHelpers.h"
#include "../HttpMessage/HttpMessage.h"

using namespace ApplicationHelpers;

class Display
{
public:
	//<-- display demonstrate message regarding project #3 -->
	static void demonstrateMessage()
	{
		show("  -> Here I am demonstrate all requirements in one case only.", always);
		show("  -> Here Client1 and Client2 sending text/binary files Server1 and Server2 that means", always);
		show("     it demonstrate requirements - 3,4,5 and 6.", always);
		show("  -> Here, Client1 sending two files one to server1 and one to server2 so, ", always);
		show("     Client1 first connects to server1 and send file then disconnects form it", always);
		show("     then send file to server2. Thus, it is first case. Moreover, Client2 also doing same", always);
		show("  -> So, Client1 and Client2 sending two files to server1 and server2 simultaneously so  ", always);
		show("     this is called as peer to peer communication.", always);
		show("     Also every server send ACK to each client1 after received file successfuly.", always);
	}

	//<-- display title with underline.-->
	static void title(const std::string& title, char ch = '=')
	{
		ApplicationHelpers::title(title, ch);
		show("\n");
	}

	//<-- display received file information.-->
	static void displayReceviedFile(std::string fileName, std::string IP = "", std::string port = 0, bool isUploaded = false)
	{
		if(isUploaded)
			Verbose::show("\n " + fileName + " is uploaded Successfully at " + IP + ":" + port + "\n", always);
		else
			Verbose::show("\n " + fileName + " is received successfully from " + IP + ":" + port + "\n", always);
	}

	//<-- display send file information.-->
	static void displaySendInformation(std::string str, std::string destIP, int destPort, bool isFile = true, bool isString = true)
	{
		if(isFile)
			Verbose::show("\n File \"" + str + "\" is going to upload on " + destIP + ":" + std::to_string(destPort) + "\n", always);
		else if(isString)
			Verbose::show("\n " + str + " byte of string sent on " + destIP + ":" + std::to_string(destPort) + "\n", always);
		else
			Verbose::show("\n " + str + " byte of buffer data sent on " + destIP + ":" + std::to_string(destPort) + "\n", always);
	}

	//<-- display error log information.-->
	static void displayErrorLog(std::string message)
	{
		Verbose::show("\n", always);
		Verbose::show(message, always);
		Verbose::show("\n", always);
	}
	//<-- display Http Message information.-->
	static void displayHttpMessage(std::string message, std::map<std::string, std::string> headerData)
	{
		Verbose::show("\n", always);
		Verbose::show(" **   Http Message   ** ", always);
		Verbose::show("Command Type :" + headerData[COMMAND_TYPE], always);
		Verbose::show("Source Add   :" + headerData[SOURCE_ADD], always);
		Verbose::show("Source Port  :" + headerData[SOURCE_PORT], always);
		Verbose::show("Dest Add     :" + headerData[DESTINATION_ADD], always);
		Verbose::show("Dest Port    :" + headerData[DESTINATION_PORT], always);
		Verbose::show("File Name    :" + headerData[FILE_NAME], always);
		Verbose::show("Content len  :" + headerData[CONTENT_LENGTH], always);
		Verbose::show("Message Body :" + message, always);
		Verbose::show("\n", always);

	}
	//<-- display norml log message information.-->
	static void show(std::string message, bool isAlways = false)
	{
		if(isAlways)
			Verbose::show(message, always);
		else
			Verbose::show(message);
	}

};
#endif

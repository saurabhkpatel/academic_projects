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
* static void displayReceviedFile(std::string fileName) //display received file information
* static void show(std::string message, bool isAlways=false) //	dsplay norml log message information
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
	//<-- display received file information.-->
	static void displayReceviedFile(std::string fileName, std::string IP = "", std::string port = 0, bool isUploaded = false)
	{
		if(isUploaded)
		{
			std::string temp = "\n " + fileName + " is uploaded Successfully at " + IP + ":" + port + "\n";
			std::cout << temp;
		}
		else
		{
			std::string temp = "\n " + fileName + " is received successfully from " + IP + ":" + port + "\n";
			std::cout << temp;
		}
			
	}
	//<-- display norml log message information.-->
	static void show(std::string message, bool isAlways = false)
	{
		if(isAlways)
			std::cout << message << std::endl;
	}

};
#endif

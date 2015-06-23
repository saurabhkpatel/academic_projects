#ifndef MESSAGE_H
#define MESSAGE_H

/////////////////////////////////////////////////////////////////////////////
// HttpMessage.h - This package create and interprets http message for     //
//                 Client and Server's communication.                      //
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
* This package provides functions related to Htpp Message pack and unpack.When ever client or server
* wants to send data to any of them then they will send message in form of Http Message.
* Http message header seems like this :
*                  HttpMessage :     Attibutes|Body
* attribute1 splitter attribute2 splitter attribute3 splitter attribute4 splitter body EndofBodyCharacter
* Here i used ETX ('3') as Seprator and as of End of body i used EOT('4')
*
* Public Interface:
* ==============================
* std::string Message::createHeaderMessage(Message msg); // create string message from Message object class.
* std::string createOperation(std::string commandType, std::string fileName, std::string destAdd, int destPort,std::string body=""); // create operational message
* std::vector<std::string> split(std::string message, char seperator); // create vector from message using sperator.
* std::map<std::string, std::string> createMap(std::vector<std::string> headerAttributes); // generate map from vector.
* Message parseMessage(std::string message);// Parse Message from string message. Convert string message to Messsage object.
*
* Required Files:
* ============================
* HttpMessage.cpp , HttpMessage.h
*
* Build Command:
* ============================
* devenv RemoteCodeManagement.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 1.0 : 9 March 15
* - first release
*/

#include<string>
#include<vector>
#include<iostream>
#include<map>
#include<algorithm>
#include<sstream>

enum action_byte
{
	ETX = 0x03, ///end of text = 0x03H
//	EOT = 0x04, /// End of Transmission 
};

// Buffer Size
#define BUFFER_SIZE  1024
#define KEY_VALUE_PAIR_SEP '|'
#define FILES_SEP '$'

// command types
#define UPLOAD_STARTED "01"
#define UPLOAD_COMPLETED "02"
#define UPLOAD_ACK "03"

#define DOWNLOAD_REQUEST "20"
#define DOWNLOAD_STARTED "21"
#define DOWNLOAD_COMPLETED "22"
#define DOWNLOAD_ACK "23"

#define SENT_STRING_TEST "05"
#define SENT_STRING_ACK "06"
#define SENT_BUFFER_TEST "07"
#define SENT_BUFFER_ACK "08"

#define SENT_HTTPMESSAGE_TEST "09"
#define SENT_HTTPMESSAGE_TEST_ACK "10"

#define GETFILES_REQUEST "11"
#define GETFILES_RESPONSE "12"

#define TEXTSEARCH_REQUEST "13"
#define TEXTSEARCH_RESPONSE "14"

// Attributes keys
#define COMMAND_TYPE "commandtype"
#define SOURCE_ADD "sourceaddress"
#define SOURCE_PORT "sourceport"
#define DESTINATION_ADD "destinationaddress"
#define DESTINATION_PORT "destinationport"
#define FILE_NAME "filename"
#define OPERATION_BODY "operation_body"
#define CONTENT_LENGTH "contentlength"
#define PATTERNS "patterns"
#define CATEGORY "category"

// Operation names,
#define TEST_MESSAGE "testMessage"
#define UPLOAD "upload"
#define DOWNLOAD "download"
#define GETFILES "getfiles"
#define TEXTSEARCH "textSearch"
#define SEND_STRING "sentstring"
#define SEND_BUFFER "sentbuffer"



class Message
{
public:
	std::string Message::createHeaderMessage(Message msg);
	std::string createOperation(std::string commandType, std::string fileName, std::string destAdd, int destPort,std::string body="");
	std::vector<std::string> split(std::string message, char seperator);
	std::map<std::string, std::string> createMap(std::vector<std::string> headerAttributes);
	Message parseMessage(std::string message);

	std::string commandType;
	std::string sourceAddress;
	std::string destAdd;
	std::string fileName;
	std::string patterns;
	std::string textSearch;
	std::string catagoryType;
	int senderPort;
	int destPort;
	int size = 0;
	std::string body = "";

};
#endif
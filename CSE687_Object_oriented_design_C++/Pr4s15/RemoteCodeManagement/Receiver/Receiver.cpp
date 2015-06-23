/////////////////////////////////////////////////////////////////////////////
// Receiver.cpp - This package handles operations related to do once it    //
//              recevied any request from client.                          //
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project3S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////


#include <fstream>
#include <direct.h>
#include <string>
#include <iostream>
#include "Receiver.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Display/Display.h"
#include "../DataStore/DataStore.h"
#include "../FileManager/FileManager.h"
#include "../CatalogueTextSearch/SearchText.h"
#define GetCurrentDir _getcwd
// <--- swap address and port while sending back to client.->
void swapAddressPort(Message &msg)
{
	std::string temp = msg.sourceAddress;
	msg.sourceAddress = msg.destAdd;
	msg.destAdd = temp;
	int tempPort = msg.senderPort;
	msg.senderPort = msg.destPort;
	msg.destPort = tempPort;
}

// <---Get file name from full path-->
std::string getFileName(std::string path)
{
	std::string filename;
	size_t pos = path.find_last_of("//");
	if(pos != std::string::npos)
		filename.assign(path.begin() + pos + 1, path.end());
	else
	{
		pos = path.find_last_of("\\");
		if(pos != std::string::npos)
			filename.assign(path.begin() + pos + 1, path.end());
		else
			filename = path;
	}
	return filename;
}

//<-- file write operations for upload/download->
bool Receiver::fileWrite(Socket& socket, Message& rcvmessage, std::string filePath)
{
	bool flag = false;
	Socket::byte buffer[BUFFER_SIZE];
	std::ofstream* outFile = new std::ofstream;
	outFile->open(filePath, std::ios::out | std::ios::binary);
	if(outFile->is_open())
	{
		int size = rcvmessage.size;
		bufferReading(socket, size, buffer);
		outFile->write(buffer, size);
		if(rcvmessage.commandType == UPLOAD_COMPLETED || rcvmessage.commandType == DOWNLOAD_COMPLETED)
		{
			flag = true;
			if(outFile)
			{
				outFile->close();
				outFile = nullptr;
			}
		}
		while(!flag)
		{
			std::string command = socket.recvString();
			if(command.size() == 0)
				break;
			rcvmessage = rcvmessage.parseMessage(command);
			size = rcvmessage.size;
			bufferReading(socket, size, buffer);
			outFile->write(buffer, size);
			if(rcvmessage.commandType == UPLOAD_COMPLETED || rcvmessage.commandType == DOWNLOAD_COMPLETED)
			{
				flag = true;
				if(outFile)
				{
					outFile->close();
					outFile = nullptr;
				}
			}
		}
	}
	else
		Display::show("\n file can not open, there might be issue of File path.", true);
	return flag;
}

//<-- file upload operation done here.-->
bool Receiver::fileUpload(Socket& socket, Message& rcvmessage)
{
	bool flag = false;
	std::string temp = rcvmessage.fileName;
	std::string onlyFileName = getFileName(temp);
	if(rcvmessage.commandType == DOWNLOAD_COMPLETED || rcvmessage.commandType == DOWNLOAD_STARTED)
		temp = CLIENTROOTPATH + onlyFileName;
	else if(rcvmessage.catagoryType.compare("root") != 0)
		temp = rcvPath_ + rcvmessage.catagoryType + "/" + onlyFileName;
	else 
		temp = rcvPath_ + onlyFileName;
	flag = fileWrite(socket,rcvmessage, temp);
	return flag;
}

//<-- buffer reading, it reads data in form of buffer from socket.-->
bool Receiver::bufferReading(Socket& socket_, int size, Socket::byte* buffer)
{
	bool ok;
	if(socket_.bytesWaiting() == 0)
		return true;
	ok = socket_.recv(size, buffer);
	if(socket_ == INVALID_SOCKET || !ok)
		return false;
	return true;
}

//<-- string receive handler, ACK and normal string receive -->
void Receiver::getServerFiles(Message& rcvmessage)
{
	if((rcvmessage.commandType == GETFILES_REQUEST))
	{
		std::string path = rcvPath_;
		if(rcvmessage.catagoryType.compare("root") != 0)
			path = rcvPath_ + rcvmessage.catagoryType + "/";
		DataStore ds;
		std::vector<std::string> patt = rcvmessage.split(rcvmessage.patterns, ' ');
		FileManager fm(path, patt, ds);
		fm.search(true);
		std::vector<std::string> files;
		for(auto& item : ds)
		{
			std::string file = item.first;
			for(auto filePathIter : item.second)
			{
				std::string fullFilePath = (*filePathIter).c_str();
				fullFilePath = fullFilePath + "\\";
				fullFilePath = fullFilePath + file;
				files.push_back(fullFilePath);
			}
		}
		std::string fileResults = "";
		for(auto file : files)
		{
			fileResults = fileResults + file;
			fileResults.append(1, FILES_SEP);
		}
		std::string body = fileResults.substr(0, fileResults.size() - 1);
		rcvmessage.commandType = GETFILES_RESPONSE;
		rcvmessage.body = body;
		swapAddressPort(rcvmessage);
		std::string message = rcvmessage.createHeaderMessage(rcvmessage);
		qserver.enQ(message);
		Display::show("\n File Search list sent to " + rcvmessage.destAdd + ":" + std::to_string(rcvmessage.destPort)+"\n", true);
	}
	else if(rcvmessage.commandType == GETFILES_RESPONSE)
	{
		std::string message = rcvmessage.createHeaderMessage(rcvmessage);
		qserver.enQ(message);
		Display::show("\n File Search list received from " + rcvmessage.sourceAddress + ":" + std::to_string(rcvmessage.senderPort), true);
	}
}

//<-- Text Search handler-->
void Receiver::textSearchHandler(Message& rcvmessage)
{
	if(rcvmessage.commandType == TEXTSEARCH_REQUEST)
	{
		DataStore ds;
		std::vector<std::string> patt = rcvmessage.split(rcvmessage.patterns, ' ');
		std::string path = rcvPath_;
		if(rcvmessage.catagoryType.compare("root") != 0)
			path = rcvPath_ + rcvmessage.catagoryType + "/";
		FileManager fm(path, patt, ds);
		fm.search(true);

		std::list<std::string> listOfFiles;
		SearchText sTxt(listOfFiles);
		sTxt.searchText(rcvmessage.textSearch, ds, patt, patt);

		if(listOfFiles.size() > 0)
		{
			std::string fileResults = "";
			for(auto file : listOfFiles)
			{
				fileResults = fileResults + file;
				fileResults.append(1, FILES_SEP);
			}
			std::string body = fileResults.substr(0, fileResults.size() - 1);
			rcvmessage.body = body;
		}
		rcvmessage.commandType = TEXTSEARCH_RESPONSE;
		swapAddressPort(rcvmessage);
		std::string message = rcvmessage.createHeaderMessage(rcvmessage);
		qserver.enQ(message);
		Display::show("\n File list after text search sent to " + rcvmessage.destAdd + ":" + std::to_string(rcvmessage.destPort), true);
	}
	else if(rcvmessage.commandType == TEXTSEARCH_RESPONSE)
	{
		std::string message = rcvmessage.createHeaderMessage(rcvmessage);
		qserver.enQ(message);
		Display::show("\n File list of text search received from " + rcvmessage.sourceAddress + ":" + std::to_string(rcvmessage.senderPort), true);
	}
}

//<-- File Upload handler-->
void Receiver::fileDownloadHandler(Socket& socket_, Message& rcvmessage)
{
	if(rcvmessage.commandType == DOWNLOAD_REQUEST)
	{
		Display::show(" File download request has been received by " + rcvmessage.sourceAddress + ":" + std::to_string(rcvmessage.senderPort), true);
		swapAddressPort(rcvmessage);
		std::string message = rcvmessage.createHeaderMessage(rcvmessage);
		qserver.enQ(message);
	}
	else if(rcvmessage.commandType == DOWNLOAD_STARTED || rcvmessage.commandType == DOWNLOAD_COMPLETED && rcvmessage.fileName.size() > 0)
	{

		std::string fileName = rcvmessage.fileName;
		if(fileUpload(socket_, rcvmessage))
		{
			std::string onlyFileName = getFileName(fileName);
			rcvmessage.commandType = DOWNLOAD_ACK;
			std::string message = rcvmessage.createHeaderMessage(rcvmessage);
			qserver.enQ(message);
			Display::show(" File downloaded from " + rcvmessage.sourceAddress + ":" + std::to_string(rcvmessage.senderPort), true);
		}
		else
			Display::show(" File download failed ", true);
	}
}

//<-- File Upload handler-->
void Receiver::fileUploadHandler(Socket& socket_, Message& rcvMessage)
{
	if((rcvMessage.commandType == UPLOAD_STARTED || rcvMessage.commandType == UPLOAD_COMPLETED) && rcvMessage.fileName.size() > 0)
	{
		std::string fileName = rcvMessage.fileName;
		if(fileUpload(socket_, rcvMessage))
		{
			std::string onlyFileName = getFileName(fileName);
			Display::displayReceviedFile(onlyFileName, rcvMessage.sourceAddress, std::to_string(rcvMessage.senderPort));
			swapAddressPort(rcvMessage);
			rcvMessage.commandType = UPLOAD_ACK;
			std::string message = rcvMessage.createHeaderMessage(rcvMessage);
			qserver.enQ(message);
		}
		else
			Display::show(" File upload failed ", true);
	}
	else if((rcvMessage.commandType == UPLOAD_ACK)) // get files
	{
		Display::displayReceviedFile(rcvMessage.fileName, rcvMessage.sourceAddress, std::to_string(rcvMessage.senderPort), true);
		std::string message = rcvMessage.createHeaderMessage(rcvMessage);
		qserver.enQ(message);
	}
}


// -Server or client's receive handlder class.
void Receiver::operator()(Socket& socket_)
{
	std::string command;
	while(true)
	{
		command = socket_.recvString();
		if(command.size() == 0)
			break;
		Message rcvMessage;
		rcvMessage = rcvMessage.parseMessage(command);
		if(rcvMessage.commandType == UPLOAD_STARTED || rcvMessage.commandType == UPLOAD_COMPLETED || rcvMessage.commandType == UPLOAD_ACK)
		{
			fileUploadHandler(socket_, rcvMessage);
			continue;  // go back and get another command
		}
		else if((rcvMessage.commandType == GETFILES_REQUEST) || (rcvMessage.commandType == GETFILES_RESPONSE))
		{
			getServerFiles(rcvMessage);
			continue;
		}
		else if(rcvMessage.commandType == DOWNLOAD_REQUEST || rcvMessage.commandType == DOWNLOAD_STARTED || rcvMessage.commandType == DOWNLOAD_COMPLETED)
		{
			fileDownloadHandler(socket_, rcvMessage);
		}
		else if(rcvMessage.commandType == TEXTSEARCH_REQUEST || rcvMessage.commandType == TEXTSEARCH_RESPONSE)
		{
			textSearchHandler(rcvMessage);
		}
	}
}

//----< test stub starts here >----------------------------------------------
//struct Cosmetic
//{
//  ~Cosmetic()
//  {
//      std::cout << "\n  press key to exit: ";
//      std::cin.get();
//      std::cout << "\n\n";
//  }
//} aGlobalForCosmeticAction;

//----< demonstration >------------------------------------------------------
#ifdef TEST_RECEIVER

int main()
{
	title("Testing Receiver Package", '=');
	try
	{
		BlockingQueue<std::string> queue;// = QServer<std::string, 50>().get();
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(9085, Socket::IP6);
		Receiver cp(queue);
		sl.start(cp);

		Verbose::show("press key to exit: ", always);
		std::cout.flush();
		std::cin.get();
	}
	catch(std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

#endif
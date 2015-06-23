/////////////////////////////////////////////////////////////////////////////
// Sender.cpp - This file handles operations to test client and server     //
//            communication                                                //
//																		   //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project3S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

#include "Sender.h"
#include "../Display/Display.h"
#include "../HttpMessage/HttpMessage.h"

//<-- read file from client side and send that file data to server with specified block size and header.-->
void Sender::sendFiledata(Socket& si,Message requestMessage,bool isUpload)
{
	std::ifstream infile;
	infile.open(requestMessage.fileName, std::ios::binary | std::ios::ate | std::ios::in);
	if(infile.is_open())
	{
		int size;
		char buffer[BUFFER_SIZE];
		size = static_cast<int>(infile.tellg());
		infile.seekg(0, infile.beg);
		while(size > 0)
		{
			std::memset(buffer, 0, BUFFER_SIZE);
			int bSize = 1024;
			if(size < bSize)
				bSize = size;
			size = size - bSize;
			std::string stringmessage = "";
			requestMessage.size = bSize;
			if(size == 0)
			{
				if(isUpload)
					requestMessage.commandType = UPLOAD_COMPLETED;
				else
					requestMessage.commandType = DOWNLOAD_COMPLETED;
			}
			else
			{
				if(isUpload)
					requestMessage.commandType = UPLOAD_STARTED;
				else
					requestMessage.commandType = DOWNLOAD_STARTED;
			}
			stringmessage = requestMessage.createHeaderMessage(requestMessage);
			si.sendString(stringmessage);
			infile.read(buffer, bSize);
			si.send(bSize, buffer);
			if(infile.eof())
				break;
		}
		infile.close();
	}
	else
		Display::show(requestMessage.fileName + " can not able to open for read.",true);
}

//<-- get List of uploaded files from server.-->
void Sender::sendDownloadRequest(Socket& si, Message msg)
{
	msg.commandType = DOWNLOAD_REQUEST;
	std::string requestMessage = msg.createHeaderMessage(msg);
	si.sendString(requestMessage);
}

//<-- get List of uploaded files from server.-->
void Sender::sendTextSearchRequest(Socket& si, Message msg)
{
	msg.commandType = TEXTSEARCH_REQUEST;
	std::string requestMessage = msg.createHeaderMessage(msg);
	si.sendString(requestMessage);
}

//<-- get List of uploaded files from server.-->
void Sender::getsFiles(Socket& si, Message msg)
{
	msg.commandType = GETFILES_REQUEST;
	std::string requestMessage = msg.createHeaderMessage(msg);
	si.sendString(requestMessage);
}

struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press key to exit: ";
		std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

//----< Test stud of sender class >------------------------------------------
#ifdef TEST_SENDER
int main()
{
  title("Testing Sender package ", '=');

  try
  {
	Sender sender;
    Verbose v(true);
    SocketSystem ss;
    SocketConnecter si;
	Message msg;
    while (!si.connect("localhost", 9085))
    {
      Verbose::show("client waiting to connect");
      ::Sleep(100);
    }
	// TODO sent
	msg.commandType = UPLOAD;
	msg.senderPort = 9080;
	msg.destAdd = "localhost";
	msg.destPort = 9085;
	msg.fileName = "demo.txt";
	sender.sendFiledata(si, msg, true);
	Verbose::show("\n  client calling send shutdown\n");
    si.shutDownSend();
  }
  catch (std::exception& ex)
  {
    Verbose::show("  Exception caught:", always);
    Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
  }
}

#endif

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
void Sender::sendFiledata(Socket& si, std::string fileName, std::string senderIP, std::string destIP, int senderPort , int destPort)
{
	std::ifstream infile;
	int size;
	char buffer[BUFFER_SIZE];
	infile.open(fileName, std::ios::binary | std::ios::ate | std::ios::in);
	if(infile.is_open())
	{
		Display::displaySendInformation(fileName,destIP,destPort);
		size = static_cast<int>(infile.tellg());
		infile.seekg(0, infile.beg);
		Message msg;
		while(size > 0)
		{
			std::memset(buffer, 0, BUFFER_SIZE);
			int bSize = 1024;
			if(size < bSize)
			{
				bSize = size;
			}
			size = size - bSize;
			std::string stringmessage = "";
			if(size == 0)
			{

				stringmessage = msg.createHeaderMessage(UPLOAD_COMPLETED, senderIP, destIP, fileName, senderPort, destPort, bSize);
			}
			else
			{

				stringmessage = msg.createHeaderMessage(UPLOAD_STARTED, senderIP, destIP, fileName, senderPort, destPort, bSize);
			}
			si.sendString(stringmessage);
			infile.read(buffer, bSize);
			si.send(bSize, buffer);
			if(infile.eof())
				break;
		}
		infile.close();
	}
	else
	{
		Display::displayErrorLog(fileName + " can not able to open for read.");
	}

}

//<-- get List of uploaded files from server.-->
void Sender::getFiles(Socket& si)
{
	//Message msg;
	//std::string stringmessage = "";
	//stringmessage = msg.createHeaderMessage("05", "localhost", "localhost", "", 89, 87, 0);
	//si.sendString(stringmessage);
	//std::string str = si.recvString();
	//Verbose::show("client recvd: " + str, always);
}

//<-- send string to server.-->
void Sender::sendString(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort)
{
	Message msg;
	std::string stringmessage = msg.createHeaderMessage(SENT_STRING_TEST, senderIP, destIP, "NA", senderPort, destPort, message.size(),message);
	si.sendString(stringmessage);
	Display::displaySendInformation(std::to_string(message.size()),destIP, destPort,false);
}
//<-- send htttp message to server.-->
void Sender::sendHttpMessage(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort)
{
	Message msg;
	std::string stringmessage = msg.createHeaderMessage(SENT_HTTPMESSAGE_TEST, senderIP, destIP, "NA", senderPort, destPort, message.size(), message);
	si.sendString(stringmessage);
	std::vector<std::string> vectordata =  msg.split(stringmessage, ETX);
	Display::displayHttpMessage(message, msg.createMap(vectordata));
}

//<-- send buffer to server.-->
void Sender::sendBuffer(Socket& si, std::string message, std::string senderIP, std::string destIP, int senderPort, int destPort)
{
	Socket::byte buffer[BUFFER_SIZE];
	std::memset(buffer,0,sizeof(buffer));
	for(size_t i = 0; i < message.length(); ++i)
	{
		buffer[i] = message[i];
	}
	Message msg;
	std::string stringmessage = msg.createHeaderMessage(SENT_BUFFER_TEST, senderIP, destIP, "NA", senderPort, destPort, message.length());
	si.sendString(stringmessage);
	si.send(message.length(), buffer);
	Display::displaySendInformation(std::to_string(message.size()), destIP, destPort, false,false);
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

    while (!si.connect("localhost", 9085))
    {
      Verbose::show("client waiting to connect");
      ::Sleep(100);
    }
	
	// TODO sent
	sender.sendString(si, "Hello how are you", "localhost","localhost",9086,9085);
	sender.sendBuffer(si, "Hello how are you", "localhost", "localhost", 9086, 9085);
	sender.sendFiledata(si, "../send/demo.txt", "localhost", "localhost", 9086, 9085);


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

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
#include "Receiver.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Display/Display.h"
 
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
 
//<-- file upload operation done here.-->
bool Receiver::fileUpload(Socket& socket_, std::map<std::string, std::string> headerData)
{
    bool result = false;
    bool flag = false;
    Socket::byte buffer[BUFFER_SIZE];
    std::string temp = headerData[FILE_NAME];
    std::string onlyFileName = getFileName(temp);
    temp = "./receive/" + onlyFileName;
    std::ofstream* outFile = new std::ofstream;
    outFile->open(temp, std::ios::out | std::ios::binary);
 
    int size = atoi(headerData[CONTENT_LENGTH].c_str());
    bufferReading(socket_, size, buffer);
    outFile->write(buffer, size);
    if(headerData[COMMAND_TYPE] == UPLOAD_COMPLETED)
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
        std::string command = socket_.recvString();
        if(command.size() == 0)
            break;
        Message msg;
        std::vector<std::string> attributes = msg.split(command, ETX);
        std::map<std::string, std::string> headerData = msg.createMap(attributes);
        size = atoi(headerData[CONTENT_LENGTH].c_str());
 
        bufferReading(socket_, size, buffer);
        outFile->write(buffer, size);
        if(headerData[COMMAND_TYPE] == UPLOAD_COMPLETED)
        {
            flag = true;
            if(outFile)
            {
                outFile->close();
                outFile = nullptr;
            }
        }
    }
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
void Receiver::stringReceiveHandler(std::map<std::string, std::string> headerData,std::string recvString)
{
    if((headerData[COMMAND_TYPE] == SENT_HTTPMESSAGE_TEST))
    {
        Message msg;
        std::string message = msg.createHeaderMessage(SENT_HTTPMESSAGE_TEST_ACK, headerData[DESTINATION_ADD], headerData[SOURCE_ADD], "NA", stoi(headerData[DESTINATION_PORT]), stoi(headerData[SOURCE_PORT]), stoi(headerData[CONTENT_LENGTH]));
        Display::displayHttpMessage(recvString.substr(0, recvString.size() - 1), headerData);
        qserver.enQ(message);
    }
    else if((headerData[COMMAND_TYPE] == SENT_STRING_TEST))
    {
        Message msg;
        std::string message = msg.createHeaderMessage(SENT_STRING_ACK, headerData[DESTINATION_ADD], headerData[SOURCE_ADD], "NA", stoi(headerData[DESTINATION_PORT]), stoi(headerData[SOURCE_PORT]), stoi(headerData[CONTENT_LENGTH]));
        qserver.enQ(message);
        Display::show(headerData[CONTENT_LENGTH] + " bytes of string has been received successfully.", true);
        Display::show("Received string : " + recvString.substr(0, recvString.size() - 1), true);
    }
    else if((headerData[COMMAND_TYPE] == SENT_STRING_ACK))
        Display::show(headerData[CONTENT_LENGTH] + " bytes of string has been sent successfully.", true);
    else if((headerData[COMMAND_TYPE] == SENT_HTTPMESSAGE_TEST_ACK))
        Display::show(" Http Message has been sent successfully.", true);
}
 
//<-- File Upload handler-->
void Receiver::fileUploadHandler(Socket& socket_, std::map<std::string, std::string> headerData)
{
    if((headerData[COMMAND_TYPE] == UPLOAD_STARTED || headerData[COMMAND_TYPE] == UPLOAD_COMPLETED) && headerData[FILE_NAME].size() > 0)
    {
        Message msg;
        std::string fileName = headerData[FILE_NAME];
        if(fileUpload(socket_, headerData))
        {
            std::string onlyFileName = getFileName(fileName);
            Display::displayReceviedFile(onlyFileName, headerData[SOURCE_ADD], headerData[SOURCE_PORT]);
            std::string message = msg.createHeaderMessage(UPLOAD_ACK, headerData[DESTINATION_ADD], headerData[SOURCE_ADD], fileName, stoi(headerData[DESTINATION_PORT]), stoi(headerData[SOURCE_PORT]), stoi(headerData[CONTENT_LENGTH]));
            qserver.enQ(message);
        }
    }
    else if((headerData[COMMAND_TYPE] == UPLOAD_ACK)) // get files
        Display::displayReceviedFile(headerData[FILE_NAME], headerData[SOURCE_ADD], headerData[SOURCE_PORT], true);
 
}
 
//----< Client Handler thread starts running this function >-----------------
/////////////////////////////////////////////////////////////////////////////
// Server's client handler class
// - must be callable object so we've built as a functor
// - passed to SocketListener::start(CallObject& co)
// - Client handling thread starts by calling operator()
void Receiver::operator()(Socket& socket_)
{
    std::string fileName;
    std::string command;
    Message msg;
    while(true)
    {
        command = socket_.recvString();
        if(command.size() == 0)
            break;
        std::vector<std::string> attributes = msg.split(command, ETX);
        if(attributes.size() > 0)
        {
            std::map<std::string, std::string> headerData = msg.createMap(attributes);
            if((headerData[COMMAND_TYPE] == UPLOAD_STARTED || headerData[COMMAND_TYPE] == UPLOAD_COMPLETED) || headerData[COMMAND_TYPE] == UPLOAD_ACK)
            {
                fileUploadHandler(socket_, headerData);
                continue;  // go back and get another command
            }
            else if((headerData[COMMAND_TYPE] == SENT_HTTPMESSAGE_TEST) || (headerData[COMMAND_TYPE] == SENT_STRING_TEST) || (headerData[COMMAND_TYPE] == SENT_STRING_ACK))
            {
                stringReceiveHandler(headerData, attributes[attributes.size() - 1]);
                continue;
            }
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
        Verbose v(true);
        SocketSystem ss;
        SocketListener sl(9085, Socket::IP6);
        Receiver cp;
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
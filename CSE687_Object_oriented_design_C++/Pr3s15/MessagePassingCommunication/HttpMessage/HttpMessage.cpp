#include "HttpMessage.h"

/////////////////////////////////////////////////////////////////////////////
// HttpMessage.cpp - This package create and interprets http message for   //
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

//----< construct http message with header and body >---------------

std::string Message::createHeaderMessage(std::string commandType, std::string sourceAddress, std::string destAdd, std::string fileName, int senderPort, int destPort, int size, std::string body)
{
	std::string message = COMMAND_TYPE;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += commandType;
	message.append(1, ETX);

	message += SOURCE_ADD;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += sourceAddress;
	message.append(1, ETX);

	message += SOURCE_PORT;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += std::to_string(senderPort);;
	message.append(1, ETX);

	message += DESTINATION_ADD;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += destAdd;
	message.append(1, ETX);

	message += DESTINATION_PORT;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += std::to_string(destPort);
	message.append(1, ETX);

	message += FILE_NAME;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += fileName;
	message.append(1, ETX);

	message += CONTENT_LENGTH;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += std::to_string(size);
	message.append(1, ETX);

	message += body;
	message.append(1, EOT);
	return message;
}

//----< construct operation or command message, which will be use for to construct request message >---------------

std::string Message::createOperation(std::string commandType, std::string fileName, std::string destAdd, int destPort,std::string body)
{
	std::string message = COMMAND_TYPE;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += commandType;
	message.append(1, ETX);

	message += FILE_NAME;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += fileName;
	message.append(1, ETX);

	message += DESTINATION_ADD;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += destAdd;
	message.append(1, ETX);

	message += DESTINATION_PORT;
	message.append(1, KEY_VALUE_PAIR_SEP);
	message += std::to_string(destPort);

	if(body.size() > 0)
	{
		message.append(1, ETX);
		message += OPERATION_BODY;
		message.append(1, KEY_VALUE_PAIR_SEP);
		message += body;
	}
	return message;
}

//----< split http string message in to vector of attribute strings. >---------------

std::vector<std::string> Message::split(std::string message, char seperator)
{
	std::vector<std::string> elements;
	std::stringstream ss(message);
	std::string item;
	while(std::getline(ss, item, seperator))
	{
		elements.push_back(item);
	}
	return elements;
}

//----< generates map from vector of attribute strings. >---------------

std::map<std::string, std::string> Message::createMap(std::vector<std::string> headerAttributes)
{
	std::map<std::string, std::string> attributeMap;

	for(auto item : headerAttributes)
	{
		auto vectorKeyValue = split(item, ':');
		if(vectorKeyValue.size() >= 2)
		{
			attributeMap[vectorKeyValue[0]] = vectorKeyValue[1];
		}
	}
	return attributeMap;
}

//----< Test Stub of Http Message >--------

#ifdef TEST_HTTPMESSAGE
int main()
{
	std::cout << " Demonstrate Http Message class " << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << std::endl;
	Message msg;
	std::string temp = msg.createHeaderMessage(UPLOAD_STARTED,"localhost","localhost","Test.txt",9085,9086,1024);
	std::cout << " Created message without body is : " << temp << std::endl;
	std::cout << std::endl;
	temp = msg.createHeaderMessage(UPLOAD_STARTED, "localhost", "localhost", "Test.txt", 9085, 9086, 17, "Hello how are you");
	std::cout << " Created message without body is : "<< temp << std::endl;
	std::cout << std::endl;
	std::vector<std::string> vectorOfAttributes = msg.split(temp, ETX);
	std::cout << "Http Message attributes list : " << std::endl;
	std::cout << std::endl;
	for(auto attribute : vectorOfAttributes)
	{
		std::cout << "     " <<  attribute << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Http Message attribute key-value pairs : " << std::endl;
	std::cout << std::endl;
	std::map<std::string, std::string> mapOfKeyValuePair = msg.createMap(vectorOfAttributes);
	for(auto keyvaluePair : mapOfKeyValuePair)
	{
		std::cout << "     " << keyvaluePair.first << " : " << keyvaluePair.second << std::endl;
	}
}
#endif

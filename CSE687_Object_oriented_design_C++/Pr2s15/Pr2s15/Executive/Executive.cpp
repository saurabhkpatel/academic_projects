/////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Entry point for Project #2 Demo                         //
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project2S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* - Builds a Xml Document object from xml file or string.
*
* User Interface:
* - Executive.exe [XmlFilePath] [FilePathForStringInput]
*
* Required Files:
* ---------------
* Executive.cpp, BuildTree.cpp, Tokenizer.cpp, XmlDocument.cpp, XmlElement.cpp, XmlElementParts.cpp, Display.h, XmlElement.h, XmlDocument.h
*
* Build Process:
* --------------
* From the Visual Studio Developer's Command Prompt:
* devenv Pr2s15.sln /rebuild debug
*
* Maintenance History:
* --------------------
* - Ver 1.0 : 21 Mar 2015
*   first release
*/

#include <iostream>
#include <fstream>
#include <string>
#include "../Display/Display.h"
#include "../Demonstrate/Demonstarte.h"


class Executive
{
private:
	std::string fileName = "LectureNote.xml";
	std::string stringFileName = "string.txt";
	XmlProcessing::XmlDocument *xmldocument;
public:
	Executive(int argc, char** argv);
	Executive(const Executive& ex) = delete;
	Executive& operator=(const Executive& ex) = delete;
	std::string readFileContent();
	void processCommands();
	void demonstrateFileXml();
	void demonstrateStringxml();
};

//----< check file suffix >------------------------
bool has_suffix(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

//----< Executive constructor >---------
Executive::Executive(int argc, char** argv)
{
	if(argc > 1 && argv[1] && has_suffix(argv[1], ".xml"))
		fileName = argv[1];
	if(argc > 2 && argv[2] && has_suffix(argv[2], ".txt"))
		stringFileName = argv[2];
}

//----< read file contents and return std::string>---------
std::string Executive::readFileContent()
{
	std::ifstream in(stringFileName);
	if(!in.good())
		return "";
	std::string fileText;
	while(in.good())
	{
		fileText += in.get();
	}
	in.close();
	return fileText;
}

//----< demonstrate requirements using xml file>---------
void Executive::demonstrateFileXml()
{
	DisplayData::title("   Project #2 Demonstration using xml file input", '#');
	xmldocument = new XmlProcessing::XmlDocument(fileName, XmlProcessing::XmlDocument::filename);
	if(xmldocument->getTreeDocumentElement() == NULL)
	{
		DisplayData::title("  Xml Document Object is not constructed ", '-');
	}
	else
	{
		Demonstrate demo(xmldocument);
		demo.demonstrateFileXml();
	}
	delete xmldocument;
}

//----< demonstrate requirements using string input>---------
void Executive::demonstrateStringxml()
{
	DisplayData::title("   \n\n\n   Project #2 Demonstration using xml string input", '#');
	std::string stringInput = readFileContent();
	if(stringInput.length() > 0)
	{
		xmldocument = new XmlProcessing::XmlDocument(stringInput, XmlProcessing::XmlDocument::string);
		Demonstrate demo(xmldocument);
		demo.demonstrateStringxml();
		delete xmldocument;
	}
	else
	{
		DisplayData::title("  Xml Document Object is not constructed ", '-');
	}
}

//----< process commands>---------
void Executive::processCommands()
{
	demonstrateFileXml();
	demonstrateStringxml();
}

#ifdef TEST_EXECUTIVE
int main(int argc, char* argv[])
{
	DisplayData::title("   Project #2 Demonstration:", '*');
	std::cout << "\n";
	Executive exec(argc, argv);
	exec.processCommands();
}
#endif

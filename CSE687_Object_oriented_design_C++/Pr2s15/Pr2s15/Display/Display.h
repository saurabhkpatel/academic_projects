#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h - Supports display functions.                                 //
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
* ===============================
* This package will provides functions to display the xml operations and xml internal tree representation.
*
* Public Interface:
* ==============================
* static void title(const std::string& title, char ch = '=');
*    Use to print simple provided input string.
* static void showXmlTree(const std::shared_ptr < XmlProcessing::AbstractXmlElement >& tree, bool fileWrite=true)
*    prints xml data.
* static void showData(std::vector<std::shared_ptr <XmlProcessing::AbstractXmlElement >>& result);
*    Prints xml elements using provided input vector of AbstractXmlElement pointers.
* static void showAttributesPair(const std::vector<std::pair<std::string, std::string>>& attriPairs)
*   Shows xml elements attributes pairs with key and value.
*
* Required Files:
* ============================
*  Display.cpp, Display.h, XmlElement.h
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
#include <fstream>
#include <vector>
#include "../XmlDocument/XmlElement/XmlElement.h"

class DisplayData
{
public:

	static void title(const std::string& title, char ch = '=')
	{
		std::cout << "\n " << title;
		if(ch=='=')
			std::cout << "\n " << std::string(title.size() + 2, ch);
		else
			std::cout << "\n  " << std::string(title.size() + 2, ch);
		std::cout << "\n";
	}
	
	static void showXmlTree(const std::shared_ptr < XmlProcessing::AbstractXmlElement >& tree,bool fileWrite=true,std::string fileName="")
	{
		if(tree != NULL)
		{
			std::cout << "\n " << tree->toString() << std::endl;
		}
		if(fileWrite)
		{
			if(fileName.length())
			{
				writeXmlFile(tree->toString(), fileName);
			}
			else
			{
				writeXmlFile(tree->toString());
			}
		}
	}

	static void showData(std::vector<std::shared_ptr <XmlProcessing::AbstractXmlElement >>& result)
	{
		if(result.size() > 0)
		{
			for(auto & item : result)
			{
				std::cout << "\n " << item->toString() << std::endl;
			}
		}
		else
		{
			std::cout << "  No data found !! " << std::endl;
		}
		std::cout << "\n ";
	}

	static void showAttributesPair(const std::vector<std::pair<std::string, std::string>>& attriPairs)
	{
		if(attriPairs.size() > 0)
		{
			for(auto & item : attriPairs)
			{
				std::cout << "\n    " << item.first << "="  << item.second;
			}
		}
		else
		{
			std::cout << "\n  No data found !! " << std::endl;
		}
		std::cout << "\n ";
	}
	//---used to write xml file with xml tree as string.---
	static void writeXmlFile(const std::string& output,const std::string &fileName="")
	{
		std::ofstream outFile;
		if(fileName.length() > 0)
			outFile.open(fileName);
		else
			outFile.open("output.xml");
		if(!outFile)
		{
			std::cout << "output.xml can't able to open " << std::endl;
		}
		outFile << output ;
		outFile.close();
	}
};
#endif

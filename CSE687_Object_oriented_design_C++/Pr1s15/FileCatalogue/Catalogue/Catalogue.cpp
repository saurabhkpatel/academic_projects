/////////////////////////////////////////////////////////////////////////////
// Catalogue.cpp - Parse command line options and then use these options   //
//               to build catalogue.                                       //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Saurabh Patel, 2015                                         //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project1S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

#include "Catalogue.h"
#include "../Display/Display.h" 

//<-------parse search text from argument---->
void Catalogue::parseTextsearch(const std::string& argument)
{
	size_t n = std::count(argument.begin(), argument.end(), '"');
	if(argument.size() > 1 && (n == 2 || n == 0))
	{
		if(n == 2)
		{
			size_t first = argument.find_first_of("\"");
			size_t last = argument.find_last_of("\"");

			if(!((first == std::string::npos) || (last == std::string::npos)))
			{
				std::string temp = argument.substr(first + 1, last - (first + 1));
				setSearchText(temp);
			}
		}
		else if(n==0)
		{
			setSearchText(argument.substr(2));
		}
	}
}

//<-------parse command line arguments and store them---->
void Catalogue::parseArguments(const std::string& arg, bool isFirstArgument)
{
	if(arg.find("/") != std::string::npos)
	{
		// check and set options as application got.
		if(arg.compare("/s") == 0)
			setSubDirFlag(true);
		else if(arg.compare("/d") == 0)
			setDuplicateSearchFlag(true);
		else if(arg.compare(0, std::string("/f").length(), "/f") == 0)
			parseTextsearch(arg);
		else if(isFirstArgument)
			setCataloguePath(arg);
	}
	else if(arg.find("*") != std::string::npos)
	{
		if(arg.compare("*.*") == 0)
			patterns_.clear();
		addFilePattern(arg);
	}
	else if(isFirstArgument)
		setCataloguePath(arg);
}


//<-------set options for file catalogue built up---->
void Catalogue::setOptions(int argc, char* argv[])
{
	if(argc <= 1)
	{
		// Check the value of argc. If not enough parameters have been passed, then use default parameter.
		setCataloguePath("."); // use current path
		addFilePattern("*.*"); // use default pattern
	}
	else
	{
		if(argc > 1)
		{
			addFilePattern("*.*"); // use default pattern
			setCataloguePath("."); // use current path
			for(int count = 1; count < argc; count++)
			{
				std::string arg(argv[count]);
				if(count == 1)
					parseArguments(arg, true);
				parseArguments(arg, false);
			}
		}
	}
}

#ifdef TEST_CATALOGUE

int main(int argc, char* argv[])
{
	char* n_argv[] = { "main.exe", "D:\\Test\\", "*.h", "*.cpp", "/s", "/f\"test how are you\"", "/d", "*.xml" };
	argv = n_argv;
	argc = 8;
	Catalogue cl;
	cl.setOptions(argc, argv);
	DisplayData::title("\n    Demonstrate Catalogue class.");
	DisplayData::showCatalogOptions(cl);
	return 0;
}
#endif
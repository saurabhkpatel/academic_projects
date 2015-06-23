#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h - Supports display functions.                                 //
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

/*
* Package Operations:
* ===============================
* This package will provides functions to display the contents of file catalogue,
* duplicate files and list of files from where input text search found.
*
* Public Interface:
* ==============================
* static void title(const std::string& title, char ch = '=');
*  - display input title with underline of symbol which is provided.
*  - If no ch is provided then it will take by default "=".
* static void showFileCatalouge(DataStore& dataStore);
*  - display datastore as file and path value.
* static void showDuplicateFiles(DataStore& dataStore);
*  - check and display duplicate files from datastore object.
* static void showCatalogOptions(Catalogue& ct);
*  - display provided catalog options by command line arguments.
* static void showSearchFileResults(std::list<std::string>& results);
*  - display files in which text search is found.
*
* Required Files:
* ============================
* Display.h, Display.cpp, DataStore.h, DataStore.cpp
*
* Build Command:
* ============================
* cl /EHa /DTEST_DISPLAY Display.cpp ../Datastore/Datastore.cpp
* 
* Maintenance History:
* ============================
* ver 1.0 : 10 Feb 15
* - first release
*/


#include <iostream>
#include <string>
#include "../DataStore/DataStore.h"
#include "../Catalogue/Catalogue.h"


class DisplayData
{
public:

	static void title(const std::string& title, char ch = '=')
	{
		std::cout << "\n  " << title;
		std::cout << "\n " << std::string(title.size() + 2, ch);
		std::cout << "\n";
	}

	static void showFileCatalouge(DataStore& dataStore)
	{
		std::cout << "\n";
		for(auto& item : dataStore)
		{
			std::cout << std::endl << " " << item.first.c_str() << std::endl;
			for(auto filePathIter : item.second)
			{
				std::cout << " --" << (*filePathIter).c_str() << "\n";
			}
		}
		std::cout << "\n";
		if(dataStore.size() > 0 && dataStore.pathSize() > 0)
			std::cout <<"  " << dataStore.size() << " Files found in " << dataStore.pathSize() << " directories." << "\n";
		std::cout << "\n";
	}

	static void showDuplicateFiles(DataStore& dataStore)
	{
		bool found = false;
		std::cout << "\n";
		for(auto& item : dataStore)
		{
			if(item.second.size() > 1)
			{
				found = true;
				std::cout << std::endl << " " << item.first.c_str() << std::endl;
				for(auto filePathIter : item.second)
				{
					std::cout << " --" << (*filePathIter).c_str() << "\n";
				}
			}
		}
		if(!found)
			std::cout << " --" << "No duplicate files are found." << "\n";
		std::cout << "\n";
	}

	static void showCatalogOptions(Catalogue& ct)
	{
		std::cout << "\n";
		std::cout << " --File Catalogue Path : " << std::boolalpha << ct.getCataloguePath().c_str() << "\n";
		std::cout << " --Find files in subdirectories enable? : " << std::boolalpha << ct.getSubDirFlag() << "\n";
		std::cout << " --Print duplicate files enable? : " << std::boolalpha << ct.getDuplicateSearchFlag() << "\n";
		std::cout << " --Text search keyword is : " << ct.getSearchText() << "\n";
		std::cout << " --Files patterns : ";
		for(auto item : ct.getPatterns())
		{
			std::cout << item;
		}
		std::cout << "\n";
		std::cout << "\n";
	}

	static void showSearchFileResults(std::list<std::string>& results)
	{
		std::cout << "\n";
		if(results.size() > 0)
			for(auto& item : results)
			{
			std::cout << " --" << item << "\n";
			}
		else
			std::cout << " No file are found." << "\n";
		std::cout << "\n";
	}
};
#endif

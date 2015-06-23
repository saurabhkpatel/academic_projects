/////////////////////////////////////////////////////////////////////////////
// Executor.cpp - a test executive package and display results.            //
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
* This package is a test executive where all inputs takes from users and display 
* the contents of file catalogue and give prompt to user to enter addition text search
*
* Public Interface:
* ==============================
* N/A
*
* Required Files:
* ============================
* Executor.cpp Display.h, DataStore.h, DataStore.cpp, Catalogue.h, Catalogue.cpp, FileManager.h, FileManager.cpp
* FileSystem.h, FileSystem.cpp, SearchText.cpp, SearchText.h
*
* Build Command:
* ============================
* cl /EHa Executor.cpp ../Datastore/Datastore.cpp ../FileSystem/FileSystemDemo/FileSystem.cpp ../FileManager/FileManager.cpp ../CatalogueTextSearch/SearchText.cpp ../Catalogue/Catalogue.cpp
*
* Maintenance History:
* ============================
* ver 1.0 : 10 Feb 15
* - first release
*/


#include <iostream>
#include <string>
#include <sstream>
#include "../Catalogue/Catalogue.h"
#include "../FileManager/FileManager.h"
#include "../Display/Display.h" 
#include "../CatalogueTextSearch/SearchText.h"


//<-------get patterns from input string.---->
std::vector<std::string> parsePatterns(const std::string &searchPatterns)
{
	std::stringstream ss(searchPatterns);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> patterns(begin, end);
	return patterns;
}

//<-------search text from built file catalogue---->
void searchTextFromCatalogue(Catalogue& catalogue, DataStore& dataStore, const std::vector<std::string> patterns)
{
	if(catalogue.getSearchText().size() > 0)
	{
		std::list<std::string> listOfFiles;
		SearchText sTxt(listOfFiles);
		sTxt.searchText(catalogue.getSearchText(), dataStore, catalogue.getPatterns(), patterns);
		if(listOfFiles.size() > 0)
		{
			std::string title = catalogue.getSearchText() + " text found in following files.";
			DisplayData::title(title, '-');
			DisplayData::showSearchFileResults(listOfFiles);
		}
		else
		{
			std::cout << " No files are found which contains " << catalogue.getSearchText() << + " text." << std::endl;
		}
	}
	else
	{
		std::cout << " No search text is provided." << "\n";
	}
}

//<-------built file catalogue---->
void builtFileCatalogue(int argc, char* argv[], Catalogue &catalogue, DataStore& dataStore)
{
	// parse and set command line arguments.
	catalogue.setOptions(argc, argv);
	// built up file catalog as per options
	FileManager fm(catalogue.getCataloguePath(), catalogue.getPatterns(), dataStore);
	fm.search(catalogue.getSubDirFlag());
	// catalogue build done, display file catalogue.
	DisplayData::title("\n    File Catalogue      ");
	DisplayData::showFileCatalouge(dataStore);
	// if duplicate file option present then show duplicate files also.
	if(catalogue.getDuplicateSearchFlag())
	{
		DisplayData::title("\n List of duplicate files along with path.");
		DisplayData::showDuplicateFiles(dataStore);
	}
	std::vector<std::string> patterns{ "*.*" };
	searchTextFromCatalogue(catalogue, dataStore, patterns);
}

#ifdef TEST_EXECUTOR
int main(int argc, char* argv[])
{
	try
	{
		Catalogue catalogue;
		DataStore dataStore;

		builtFileCatalogue(argc, argv, catalogue, dataStore);

		std::string userInput;
		std::cout << "\n Please enter any text which you want to search. For exit press Enter Key" << std::endl;
		while(getline(std::cin, userInput))
		{
			if(userInput.empty())
				break;
			size_t n = std::count(userInput.begin(), userInput.end(), '"');
			if(userInput.size() > 1 && (n == 2 || n == 0))
			{
				if(n == 2)
				{
					size_t first = userInput.find_first_of("\"");
					size_t last = userInput.find_last_of("\"");

					std::string searchPatterns = userInput.substr(last + 1);
					if(!((first == std::string::npos) || (last == std::string::npos)))
					{
						std::string strNew = userInput.substr(first + 1, last - (first + 1));
						catalogue.setSearchText(strNew);
					}
					std::vector<std::string>  patterns = parsePatterns(searchPatterns);
					searchTextFromCatalogue(catalogue, dataStore, patterns);
				}
				else
				{
					catalogue.setSearchText(userInput);
					std::vector<std::string> patterns{ "*.*" };
					searchTextFromCatalogue(catalogue, dataStore, patterns);
				}
			}
			std::cout << "\n Please enter any text which you want to search. For exit press Enter Key" << std::endl;
		}
	}
	catch(std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return 0;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// SearchText.cpp - search text from files, which are in file catalogue.   //
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

#include <algorithm>
#include "SearchText.h"
#include "../Display/Display.h"
#include "../FileManager/FileManager.h"
#include "../Catalogue/Catalogue.h"
#include "../FileSystem/FileSystemDemo/FileSystem.h"


//<--- check one vector is subset of other or not.--->
bool checkSubsetOfPattern(std::vector<std::string>& source, std::vector<std::string>& dest)
{
	// file catalogue already built with all patterns then do not worry about text search file patterns.
	if(find(source.begin(), source.end(), "*.*") != source.end())
		return true;
	else if(find(dest.begin(), dest.end(), "*.*") != dest.end())
		return false;
	else
	{
		std::sort(source.begin(), source.end());
		std::sort(dest.begin(), dest.end());
		return std::includes(source.begin(), source.end(), dest.begin(), dest.end());
	}

}

//<--- check file patterns, which files are matching.--->
bool checkPattern(std::string const & fileName, std::string& patt)
{
	if(patt.compare("*.*") == 0)
		return true;
	patt.erase(std::remove(patt.begin(), patt.end(), '*'), patt.end());
	if(fileName.size() < patt.size())
		return false;
	return fileName.substr(fileName.size() - patt.size()) == patt;
}

//<--- check text is found or not in given file --->
bool isTextFound(const std::string& fileName, const std::string& keyString)
{

	int offset = 0;
	FileSystem::File file(fileName);
	file.open(FileSystem::File::in);
	std::string all = file.readAll();
	if((offset = all.find(keyString, 0)) != std::string::npos)
	{
		file.close();
		return true;
	}
	file.close();
	return false;
}

//<-------search specified text in datastore file catalog.---->
void SearchText::searchText(const std::string& keyString, DataStore& dataStore, std::vector<std::string> cataloguePatterns, std::vector<std::string> searchPatterns)
{
	for(auto pattern : searchPatterns)
	{
		// check for all files which are in catalogue.
		for(auto& item : dataStore)
		{
			//check for patter is already exists or not.
			if(checkPattern(item.first, pattern))
			{
				for(auto& filePathIter : item.second)
				{
					std::string path = (*filePathIter);
					std::string fileName = path + "\\" + item.first;
					if(isTextFound(fileName, keyString))
					{
						// save file name with path in list.
						files_.push_back(fileName);
					}
				}
			}
		}
	}
}

#ifdef  TEST_SEARCHTEXT
int main()
{
	DataStore ds;
	Catalogue cl;
	cl.addFilePattern("*.txt");
	cl.setCataloguePath("../../");
	cl.setSubDirFlag(true);
	cl.setSearchText("Hello world");

	FileManager fm(cl.getCataloguePath(), cl.getPatterns(), ds);
	fm.search(cl.getSubDirFlag());

	std::list<std::string> listOfFiles;
	SearchText sTxt(listOfFiles);
	std::vector<std::string> patterns{ "*.*" };
	sTxt.searchText(cl.getSearchText(), ds, cl.getPatterns(), patterns);


	DisplayData::title("\n    Demonstrate Text Search from file Catalog files.");
	DisplayData::title("\n \"Hello world\" text found in following files", '-');
	DisplayData::showSearchFileResults(listOfFiles);

	return 0;
}
#endif
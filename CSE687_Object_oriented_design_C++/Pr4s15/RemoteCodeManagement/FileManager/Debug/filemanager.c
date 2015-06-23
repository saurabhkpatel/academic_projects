/////////////////////////////////////////////////////////////////////////////
// FileManager.cpp - Support file catalogue built operation                //
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

#include "FileManager.h"
#include "../FileSystem/FileSystemDemo/FileSystem.h"
#include "../DataStore/DataStore.h"


//<-------find files in provided input path and sub-directories is "\s" option is provided.---->
void FileManager::find(const std::string &path, bool isSubDir)
{

	FileSystem::Directory::setCurrentDirectory(path);
	for(auto patt : patterns_)
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(FileSystem::Directory::getCurrentDirectory(), patt);
		for(auto file : files)
		{
			dataStore_.addFile(file, FileSystem::Directory::getCurrentDirectory());
		}
	}
	if(isSubDir) // Boolean flag for "/s" option.
	{
		std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);
		std::vector<std::string>::iterator iter = dirs.begin();
		while(iter != dirs.end())
		{
			if(((*iter).compare(".") == 0) || ((*iter).compare("..") == 0))
			{
				// ignore "." and ".." directory.
				iter = dirs.erase(iter);
			}
			else
			{
				*iter = FileSystem::Path::fileSpec(FileSystem::Directory::getCurrentDirectory(), *iter);
				++iter;
			}
		}
		for(auto dir : dirs)
		{
			find(dir, isSubDir);
		}
	}
}

//<-------search files in input path and sub-directories if "\s" option is provided.---->
void FileManager::search(bool isSubDirFlag)
{
	std::string path = FileSystem::Path::getFullFileSpec(path_);
	find(path, isSubDirFlag);
}

#ifdef  TEST_FILEMANAGER
int main()
{
	DataStore ds;
	std::vector<std::string> patt{ "F?le*.*"};
	FileManager fm(".", patt, ds);
	fm.search(true);
	std::cout << "\n";
	return 0;
}
#endif
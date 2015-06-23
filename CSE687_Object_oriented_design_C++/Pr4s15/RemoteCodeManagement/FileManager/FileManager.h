#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/////////////////////////////////////////////////////////////////////////////
// FileManager.h - Support file catalogue built operation                  //
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
* ==================
* This module identify a set of files for analysis by supplying, on the command line, 
* a path, one or more file patterns, and sub directories analysis also, if files are 
* present, indicates that the entire directory tree rooted at the path is searched for 
* matching files.
*
*
* Public Interface:
* =================
* FileManager(const std::string& path, FilePatterns& patternVector, DataStore& ds) : path_(path), patterns(patternVector), dataStore_(ds){}
* -- This constructor takes path as input and it will be use like that from where files needs to be analyze,      datastore refernce to store
* -- Restults will be store in datastore refernce.
* void search(bool isSubDirFlag);
* -- isSubDirFlag is true then it will search files from directory and sub-directory
* -- isSubDirFlag will be true if command line "/s" is provided.
* void find(const std::string &path, bool isSubDir);
* -- find files as like provided path and isSubDir flag.
* void setPath(std::string path);
* -- Set provided path path as current directory.
* std::string getFullFilePathSpec(std::string path);
* -- get full file path spec.
* Required Files:
* ===============
* FileSystem.h, FileSystem.cpp, DataStore.cpp, DataStore.h
*
* Build Command:
* ==============
* cl /EHa /DTEST_FILEMANAGER FileManager.cpp ../Datastore/Datastore.cpp ../FileSystem/FileSystemDemo/FileSystem.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 10 Feb 15
* - first release
*/

#include <vector>
#include <iostream>
#include "../DataStore/DataStore.h"

class FileManager
{
public:

	using Iterator = DataStore::IteratorDataStore;
	using FilePatterns = std::vector < std::string > ;

	FileManager(const std::string& path, FilePatterns& patternVector, DataStore& ds) : path_(path), patterns_(patternVector), dataStore_(ds){}

	void search(bool isSubDirFlag);
	void find(const std::string &path, bool isSubDir);
	void setPath(std::string path);
	std::string getFullFilePathSpec(std::string path);
private:
	std::string path_;
	DataStore& dataStore_;
	FilePatterns patterns_;
};

#endif
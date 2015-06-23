#ifndef SEARCHTEXT_H
#define SEARCHTEXT_H

/////////////////////////////////////////////////////////////////////////////
// SearchText.h - search text from files, which are in file catalogue.     //
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
* This package will provides functions to search text from file catalogue files 
* as per provided input patterns.
*
* Public Interface:
* ==============================
* 	SearchText(std::list<std::string>& files) : files_(files) {}
*   void searchText(const std::string& keyString, DataStore& dataStore, std::vector<std::string>      cataloguePatterns, std::vector<std::string> searchPatterns);
*
* Required Files:
* ============================
* SearchText.h, SearchText.cpp, DataStore.h, DataStore.cpp, 
* FileManager.cpp, FileManager.h, FileSystem.h, FileSystem.cpp
*
* Build Command:
* ============================
* cl /EHa /DTEST_SEARCHTEXT SearchText.cpp ../Datastore/Datastore.cpp ../FileSystem/FileSystemDemo/FileSystem.cpp ../FileManager/FileManager.cpp
* 
* Maintenance History:
* ============================
* ver 1.0 : 10 Feb 15
* - first release
*/

#include <iostream>
#include <vector>
#include "../DataStore/DataStore.h"

class SearchText
{
public:
	SearchText(std::list<std::string>& files) : files_(files) {}
	void searchText(const std::string& keyString, DataStore& dataStore, std::vector<std::string> cataloguePatterns, std::vector<std::string> searchPatterns);
private:
	std::list<std::string>& files_;
};
#endif
#ifndef DATASTORE_H
#define DATASTORE_H

/////////////////////////////////////////////////////////////////////////////
// DataStore.h - supports datastorage of file catalogue operations.        //
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
* This package construct a catalog of all files in the file set, saving each file name only once
* and saving each path only once, while preserving all of the containment relationships between
* directories and their files. That implies that each file storage will have to save a list of
* references into a set of paths where they are found. You will find DataStore class inherits
* iterators property so we can use this in other class to iterate data also.
*
*
* Public Interface:
* =================
* void addFile(const std::string& filename, const std::string& filePath);
  -- This function takes filename and filepath as input and store them into map.
* IteratorDataStore begin(){ return dataStoreMap.begin();}
* IteratorDataStore end(){ return dataStoreMap.end(); }
* size_t size() { return dataStoreMap.size(); }
* size_t pathSize() { return paths.size(); }
*
* Required Files:
* ===============
* DataStore.h, DataStore.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_DATASTORE DataStore.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 10 Feb 15
* - first release
*/

#include <iostream>
#include <set>
#include <list>
#include <map>

class DataStore 
{
public:
	using Paths = std::set < std::string > ;
	using PathIters = Paths::iterator;
	using ListOfPathsIters = std::list < PathIters >;
	using Store = std::map < std::string, ListOfPathsIters >;
	using IteratorDataStore = Store::iterator;
	void addFile(const std::string& filename, const std::string& filePath);
	IteratorDataStore begin(){ return dataStoreMap.begin();}
	IteratorDataStore end(){ return dataStoreMap.end(); }
	size_t size() { return dataStoreMap.size(); }
	size_t pathSize() { return paths.size(); }
private:
	Store dataStoreMap;
	Paths paths;
};
#endif

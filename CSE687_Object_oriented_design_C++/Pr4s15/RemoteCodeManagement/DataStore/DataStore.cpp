/////////////////////////////////////////////////////////////////////////////
// DataStore.cpp - supports datastorage of file catalogue operations.      //
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

#include "DataStore.h"

//----< add file and its iterator in map >---------------------
void DataStore::addFile(const std::string& filename, const std::string& filePath)
{
	paths.insert(filePath);

	if(dataStoreMap.count(filename) > 0)
	{
		// update entry in map
		IteratorDataStore itr = dataStoreMap.find(filename);
		if(itr != dataStoreMap.end())
		{
			dataStoreMap[filename].push_back(paths.find(filePath));
		}
	}
	else
	{
		// make new entry in map
		ListOfPathsIters temp;
		temp.push_back(paths.find(filePath));
		dataStoreMap.insert(make_pair(filename, temp));
	}
}

#ifdef  TEST_DATASTORE
int main()
{

	DataStore ds;
	ds.addFile("demo.txt", "path1");
	ds.addFile("demo.txt", "path2");
	ds.addFile("demo1.txt", "path1");
	ds.addFile("demo1.txt", "path2");
	ds.addFile("demo2.txt", "path1");
	ds.addFile("demo2.txt", "path2");
	ds.addFile("demo2.txt", "path3");

	std::cout << "\n";
}
#endif
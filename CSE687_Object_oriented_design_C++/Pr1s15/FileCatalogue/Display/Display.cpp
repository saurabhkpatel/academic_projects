#include "display.h"
#include "../DataStore/DataStore.h"

/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Supports display functions.                               //
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

#ifdef TEST_DISPLAY

int main(int argc, char* argv[])
{
	DisplayData::title("  Display title demo demonstrate",'_');
	DataStore ds;
	ds.addFile("demo.txt", "path1");
	ds.addFile("demo.txt", "path2");
	ds.addFile("demo1.txt", "path1");
	ds.addFile("demo1.txt", "path2");
	ds.addFile("demo2.txt", "path1");
	ds.addFile("demo2.txt", "path2");
	ds.addFile("demo2.txt", "path3");

	DisplayData::title("  Display showFileCatalouge ", '_');

	DisplayData::showFileCatalouge(ds);

	DisplayData::title("  Display duplicate files ", '_');
	DisplayData::showDuplicateFiles(ds);

	Catalogue cl;
	cl.setCataloguePath(".");
	cl.setDuplicateSearchFlag(true);
	cl.setSearchText("test text");
	cl.setSubDirFlag(true);
		
	DisplayData::title("  Display file catalogue options ", '_');
	DisplayData::showCatalogOptions(cl);

	DisplayData::title(" Display list of files in which text search found ", '_');
	std::list<std::string> listOfFiles = {"foo.txt","baar.txt","test.txt","number.txt"};
	DisplayData::showSearchFileResults(listOfFiles);
}

#endif

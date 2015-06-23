#ifndef CATALOGUE_H
#define CATALOGUE_H

/////////////////////////////////////////////////////////////////////////////
// Catalogue.h - Parse command line options and then use these options     //
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

/*
* Package Operations:
* ===============================
* This package sets different command line argumnets and options which has been provided by
* users, and these will be use at the time of built file catalogue.
*
* Public Interface:
* ==============================
* void setOptions(int argc, char* argv[]);
* -- This take command line arguments and set options after parsing them.
* void parseArguments(const std::string& arg ,bool isFirstArgument);
* -- parse command line arguments.
* void parseTextsearch(const std::string& argument);
* -- parse /f argument and set search text.
* void setSubDirFlag(bool isSubDir);
* bool getSubDirFlag();
* void setDuplicateSearchFlag(bool isDuplicateSearch) ;
* bool getDuplicateSearchFlag();
* void setCataloguePath(const std::string& path);
* std::string getCataloguePath();
* void setSearchText(const std::string& key);
* std::string getSearchText();
* void addFilePattern(const std::string& patt);
* std::vector < std::string > getPatterns();
*
* Required Files:
* ============================
* Catalogue.h, Catalogue.cpp
*
* Build Command:
* ============================
* cl /EHa /DTEST_CATALOGUE Catalogue.cpp
* 
* Maintenance History:
* ============================
* ver 1.0 : 10 Feb 15
* - first release
*/

#include <iostream>
#include <string>
#include <vector>

class Catalogue
{
public:

	using patterns = std::vector < std::string >;
	void setOptions(int argc, char* argv[]);
	void parseArguments(const std::string& arg ,bool isFirstArgument);
	void parseTextsearch(const std::string& argument);
	void setSubDirFlag(bool isSubDir) { isSubDir_ = isSubDir; }
	bool getSubDirFlag() { return isSubDir_; }
	void setDuplicateSearchFlag(bool isDuplicateSearch) { isDuplicateSearch_ = isDuplicateSearch; }
	bool getDuplicateSearchFlag() { return isDuplicateSearch_; }
	void setCataloguePath(const std::string& path) { cataloguePath = path; }
	std::string getCataloguePath() { return cataloguePath; }
	void setSearchText(const std::string& key) { searchTextKey = key; }
	std::string getSearchText() { return searchTextKey; }

	void addFilePattern(const std::string& patt)
	{
		if(patterns_.size() == 1 && patterns_[0] == "*.*")
			patterns_.pop_back();
		patterns_.push_back(patt);
	}
	std::vector < std::string > getPatterns()
	{
		return patterns_;
	}
private:
	std::string cataloguePath;
	std::string searchTextKey;
	std::string filePattern;
	patterns patterns_;
	bool isSubDir_ = false;
	bool isDuplicateSearch_ = false;
};

#endif
///////////////////////////////////////////////////////////////////////////////
// Catalog.cpp - Build catalog of files, show results                        //
//                                                                           //
// Application:   Project #1, CSE687 - Object Oriented Design, Spring 2014   //
// Platform:      Dell XPS 2720, Win 8.1, Visual Studio 2013                 //
// Author:        Jim Fawcett, CST 4-187, Syracuse University                //
//                (315) 443-3948, jfawcett@twcny.rr.com                      //
///////////////////////////////////////////////////////////////////////////////

#include "Catalog.h"
#include "../FileMgr/FileSystem.h"
#include <fstream>
#include <sstream>

//----< reset file search path >-------------------------------------

bool Catalog::changePath(const std::string& path)
{
  if (!FileSystem::Directory::exists(path))
    return false;
  path_ == path;
  fileManager_.changePath(path);
  return true;
}
//----< search for files matching patterns and add to catalog >------

void Catalog::buildCatalog(bool recurse)
{
  fileManager_.search(recurse);
}
//----< show all files in a specified directory tree >---------------

void Catalog::showFiles()
{
  for (auto fs : store_)
  {
    std::string file = fs.first;
    std::cout << "\n  " << file;
    DataStore::PathCollection paths = store_.getPaths(file);
    for (auto path : paths)
      std::cout << "\n    " << path;
  }
}
//----< show duplicate files found in specified directory tree >-----

void Catalog::showDuplicates()
{
  for (auto f : store_)
  {
    if (f.second.size() > 1)
    {
      std::cout << "\n  " << f.first;
      for (auto iter : f.second)
        std::cout << "\n    " << *iter;
    }
  }
}
//----< does this file contain specified text? >---------------------
/*
*  ToDo:
*  - support ignoring case
*  - merge contiguous whitespace into single space
*  - support regular expressions
*  - with all these features should have its own package
*/
bool Catalog::textSearch(const std::string& filespec, const std::string& text)
{
  std::ifstream in(filespec);
  if (!in.good())
    return false;
  std::string fileText;
  while (in.good())
  {
    fileText += in.get();
  }
  in.close();
  size_t pos = fileText.find(text);
  return pos < fileText.size();
}
//----< show all files in catalog that contain specified text >------

void Catalog::showSearchResults(const std::string& searchString)
{
  bool found;
  for (auto fs : store_)
  {
    found = false;
    std::string file = fs.first;
    std::ostringstream out;
    out << "\n  " << file;
    for (auto iter : fs.second)
    {
      std::string path = *iter;
      std::string fileSpec = path + "\\" + file;
      if (textSearch(fileSpec, searchString))
      {
        out << "\n    " << path;
        found = true;
      }
    }
    if (found)
      std::cout << out.str();
  }
}
//----< show summary of findings >-----------------------------------

void Catalog::showSummary()
{
  std::cout << "\n  Found " << store_.numberOfFiles() << " files in " 
            << store_.numberOfPaths() << " directories" << "\n";
}

//----< test stub >--------------------------------------------------

#ifdef TEST_CATALOG

int main()
{
  title("Testing Catalog Package");
  bool recurse = true;
  Catalog cat("../..");
  cat.addPattern("*.h");
  cat.addPattern("*.cpp");
  cat.buildCatalog(recurse);
  cat.showSummary();
  std::cout << "\n";
  title("Files found:");
  cat.showFiles();
  title("Duplicates found:");
  cat.showDuplicates();

  std::string text = "Jim Fawcett";
  title("show textsearch for \"" + text + "\"");
  cat.showSearchResults(text);
  std::cout << "\n\n";
}

#endif

#ifndef DEMONSTRATE_H
#define DEMONSTRATE_H

/////////////////////////////////////////////////////////////////////////////
// Demonstrate.h - supports functions to demonstrate project #2 requirements//
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project2S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* - This package using Xml document object to queried for information about the XML contents, 
*   supports programmatic modification of the Document, and can write the revisions to another 
*   XML string or file.
* - Display xml tree representation
* - Supports add,remove and modification related operations for xml document object.
*
* Public Interface:
* -------------------
*	void demonstrateFileXml();      // demonstrate project 2 requirements using xml File.
*	void demonstrateStringxml();    // demonstrate project 2 requirements using string input.
* 
* Required Files:
* ---------------
* Demonstrate.h, Demonstrate.cpp, BuildTree.cpp, Tokenizer.cpp, XmlDocument.cpp, XmlElement.cpp, XmlElementParts.cpp, Display.h, XmlElement.h, XmlDocument.h
*
* Build Command:
* --------------
*
* Maintenance History:
* --------------------
* - Ver 1.0 : 21 Mar 2015
*   first release
*/

#include <iostream>
#include <fstream>
#include <string>
#include "../Display/Display.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"


class Demonstrate
{
private:
	void demonstrateBuildTree();
	void demonstrateFindAttribute();
	void demonstrateFindCollections();
	void demonstrateAddRemoveChild();
	void demonstrateRootElement();
	void demonstrateAddRemoveAttribute();
	void demonstrateChildrenAndAttributes();
	void demonstrateMove();
	XmlProcessing::XmlDocument *xmldocument;
public:
	Demonstrate(XmlProcessing::XmlDocument *doc)
	{
		xmldocument = doc;
	}
	Demonstrate(const Demonstrate& ex) = delete;
	Demonstrate& operator=(const Demonstrate& ex) = delete;

	void demonstrateFileXml();
	void demonstrateStringxml();
};

#endif
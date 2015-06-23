#ifndef BUILDXMLTREE_H
#define BUILDXMLTREE_H

/////////////////////////////////////////////////////////////////////////////
// BuildXmlTree.h- build and configure xml parser rules and build xml      //
//                 document object.                                        //
//																		   //
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
* ===============================
* This package will provides functions to parse xml tokens from input file
* and configure xml parsing rules & actions and based on that it will build xml document object.
*
* Note : Code Inspired from Dr. Jim Fawcett's parser sample code.
*
* Public Interface:
* ==============================
* bool parse();                  // run all rules which has been configured.
* bool attachXmlData(const std::string& name, bool isFile = true); // open input xml file and get xml tokens.
* bool buildXml();       // configure all rules and parse xml file and get tokens from xml file.
*
* Private functions
* ==============================
* bool parseXmlTokens();         // parse xml tokens from xml file.
* bool next();                   // to get next line of xml tokens from Tokenizer
* std::string show();                   // print xml tokens
* void addRule(IRule* pRule);    // to register rule and add in rules list.
* void parseTokens(const std::string &string); // Parse token string and divide in to vector.
*
* Required Files:
* ============================
* BuildXmlTree.h, DetectAndAction.h, Interfaces.h
* BuildXmlTree.cpp, Tokenier.cpp, XmlDocument.cpp, XmlElemement.cpp, XmlElementParts.cpp
*
* Build Command:
* ============================
* devenv Pr2s15.sln /rebuild debug
*
* Maintenance History:
* ============================
* ver 1.0 : 21 March 15
* - first release
*/

#include <vector>
#include <iostream>
#include <string>
#include "../XmlElementParts/XmlElementParts/Tokenizer.h"
#include "../XmlElementParts/XmlElementParts/itokcollection.h"
#include "Interfaces.h"
#include "DetectAndActions.h"

class TokenParser
{
public:
	TokenParser();
	~TokenParser();
	bool parse();
	bool attachXmlData(const std::string& name, bool isFile = true);
	bool buildXml();
private:
	void addRule(IRule* pRule);
	bool parseXmlTokens();
	bool next();
	std::string show();
	void parseTokens(const std::string &string);

	ITokCollection* pTokColl;
	std::vector<IRule*> detectRules;
	Toker* pToker;
	// hold xml token string and xmlParseTokes holds tokens
	std::vector<std::string> xmlParseData;
	std::vector<std::string> xmlParseTokes;
	// Add Rules and Actions.
	XmlDeclarElementStatement* decXml;
	XmlDeclarElementStatementAction* decXmlAction;
	XmlProcsElementStatement* procXml;
	XmlProcsElementStatementAction* procXmlAction;
	XmlCommentElementStatement* commentXml;
	XmlCommentElementStatementAction* commentXmlAction;
	XmlTaggedElementStatement* taggedXml;
	XmlTaggedElementStatementAction* taggedXmlAction;
	XmlTextElementStatement* textXml;
	XmlTextElementStatementAction* textXmlAction;
	// prohibit copies and assignments because it contains pointers as data members
	TokenParser(const TokenParser&);
	TokenParser& operator=(const TokenParser&);
};

#endif
/////////////////////////////////////////////////////////////////////////////
// BuildXmlTree.cpp- build and configure xml parser rules and build xml    //
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

#include "BuildXmlTree.h"
#include "../XmlElementParts/XmlElementParts/xmlElementParts.h"
#include <iostream>
#include <algorithm>
#include <sstream>

//----< TokenParser cstr >--------------------------------
TokenParser::TokenParser() : pTokColl(0), pToker(0), decXml(0), decXmlAction(0),
procXml(0), procXmlAction(0), commentXml(0), commentXmlAction(0), taggedXml(0), taggedXmlAction(0), textXml(0), textXmlAction(0)
{
}

//----< TokenParser dstr >--------------------------------
TokenParser::~TokenParser()
{
	delete procXmlAction;
	delete procXml;
	delete decXmlAction;
	delete decXml;
	delete pToker;
	delete commentXml;
	delete commentXmlAction;
	delete taggedXml;
	delete taggedXmlAction;
	delete textXml;
	delete textXmlAction;
}

//----< get next tokens string >-------
bool TokenParser::next()
{
	return pTokColl->get();
}

//----< print current tokens string >---------
std::string TokenParser::show()
{
	return pTokColl->show();
}
//----< attach file name, it would get xml data from file. >--------
bool TokenParser::attachXmlData(const std::string& name, bool isFile)
{
	if(pToker == 0)
		return false;
	pToker->setMode(Toker::xml);
	return pToker->attach(name, isFile);
}

//----< split string in tokens wrt " " >--------
void TokenParser::parseTokens(const std::string &string)
{
	xmlParseTokes.clear();
	std::stringstream ss(string);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> patterns(begin, end);
	std::string prefix = "\"";
	std::string suffix = "\"";
	std::string temp = "";
	bool found = false;
	for(auto i : patterns)
	{
		if(i[0] == '\"' && i[i.size() - 1] == '\"')
		{
			xmlParseTokes.push_back(i);
		}
		else if(i[0] == '\"')
		{
			temp += i + " ";
			found = true;
		}
		else if(i[i.size() - 1] == '\"' && found)
		{
			temp += i;
			xmlParseTokes.push_back(temp);
			found = false;
		}
		else
		{
			if(found)
			{
				temp += i + " ";
			}
			else
			{
				xmlParseTokes.push_back(i);
			}
		}
	}
}

//----< register parsing rule >--------------------------------
void TokenParser::addRule(IRule* pRule)
{
	detectRules.push_back(pRule);
}

//----< gather token string in vector >--------
bool TokenParser::parseXmlTokens()
{
	xmlParseData.clear();
	while(next())
	{
		xmlParseData.push_back(show());
	}
	return xmlParseData.size() > 0 ? true : false;
}

//----< parse the xml tokens by applying all rules to it >--------
bool TokenParser::parse()
{
	bool succeeded = false;
	if(parseXmlTokens())
	{
		for(size_t i = 0; i < xmlParseData.size(); i++)
		{
			for(size_t j = 0; j < detectRules.size(); ++j)
			{
				// split with space and put in one vector.
				parseTokens(xmlParseData[i]);
				if(detectRules[j]->detectExpression(xmlParseTokes))
				{
					succeeded = true;
					break;
				}
			}
		}
	}
	return succeeded;
}

//----< register action with a rule >--------------------------
void IRule::addAction(IAction *pAction)
{
	actions.push_back(pAction);
}

//----< invoke all actions associated with a rule >------------
void IRule::doActions(std::vector<std::string> &tokens)
{
	if(actions.size() > 0)
		for(size_t i = 0; i < actions.size(); ++i)
			actions[i]->doAction(tokens);
}

//----< build and configure xml parser rules and actions >------------
bool TokenParser::buildXml()
{
	try
	{
		// configure to detect and act on preprocessor statements
		pToker = new Toker;
		// assigned derived class instance
		pTokColl = new XmlParts(pToker);

		decXml = new XmlDeclarElementStatement;
		decXmlAction = new XmlDeclarElementStatementAction();
		decXml->addAction(decXmlAction);
		addRule(decXml);

		procXml = new XmlProcsElementStatement();
		procXmlAction = new XmlProcsElementStatementAction();
		procXml->addAction(procXmlAction);
		addRule(procXml);

		commentXml = new XmlCommentElementStatement();
		commentXmlAction = new XmlCommentElementStatementAction();
		commentXml->addAction(commentXmlAction);
		addRule(commentXml);

		taggedXml = new XmlTaggedElementStatement();
		taggedXmlAction = new XmlTaggedElementStatementAction();
		taggedXml->addAction(taggedXmlAction);
		addRule(taggedXml);

		textXml = new XmlTextElementStatement();
		textXmlAction = new XmlTextElementStatementAction();
		textXml->addAction(textXmlAction);
		addRule(textXml);
		return true;
	}
	catch(std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return false;
	}
}

#ifdef TEST_BUILDXMLTREE

int main(int argc, char* argv[])
{
	try
	{
		std::cout << "\n  Testing Build Xml Tree class\n "
			<< std::string(30, '=') << std::endl;
		int srcType = 1;// 0 for file and 1 for string.
		std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
		std::string test1 = Prologue;
		test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
		TokenParser pTokenParser;
		pTokenParser.buildXml();
		if(srcType == 1)
			pTokenParser.attachXmlData("LectureNote.xml");
		else
			pTokenParser.attachXmlData(test1, false);

		pTokenParser.parse();
		std::shared_ptr < XmlProcessing::AbstractXmlElement > pDocElement_ = Repository::getInstance()->pDocElement_;

		std::cout << "\n\n  " << pDocElement_->toString() << "\n\n";
	}
	catch(std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
}
#endif
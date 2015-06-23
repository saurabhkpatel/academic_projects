#ifndef DETECTANDACTIONS_H
#define DETECTANDACTIONS_H

/////////////////////////////////////////////////////////////////////////////
// DetectAndActions.h- This file holds defination of rules and its actions //
//                     for xml parts.                                      //
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
	This module defines several action and rules classes. This class also provide
	singletone class which will be useful to share data. First xml rules should be
	test,after that actions would be performed as per rule detection.
    
	Note : Code Inspired from Dr. Jim Fawcett's parser sample code.
*
* Public Interface:
* ==============================
* static Repository* getInstance()   :   to get Instance of Repository and use stack and pDocelement instance                                        in each action's method
* bool detectExpression(std::vector<std::string> &vector) : detect xml element type based on input vector
* void doAction(std::vector<std::string> &vector) : take action on input vector and add elements in                                                             pDocelement.
* Maintenance History:
* ============================
* ver 1.0 : 21 March 15
* - first release
*/


#include <iostream>
#include <string>
#include <algorithm>
#include "Interfaces.h"
#include "../XmlDocument/XmlElement/XmlElement.h"


//////////////////////////////////////////////////////
// Here we need to share pDocElement from xmlDocument, 
// This class will hold shared_ptr of XmlProcessing::AbstractXmlElement
// This is Singleton pattern.
class Repository  
{
private:
public:
	std::stack<std::shared_ptr<XmlProcessing::AbstractXmlElement>> stack_;
	std::shared_ptr < XmlProcessing::AbstractXmlElement > pDocElement_;

	Repository& operator= (Repository&& repo)
	{
		if(this == &repo) return *this;
		stack_ = repo.stack_;
		pDocElement_ = repo.pDocElement_;
		return *this;
	}

	std::shared_ptr < XmlProcessing::AbstractXmlElement > getDocElement()
	{
		return pDocElement_;
	}
	static Repository* getInstance()
	{
		static Repository * theInstance = new Repository();
		return theInstance;
	}
};

///////////////////////////////////////////////////////////////
// rule to detect XmlDeclarElementStatement
class XmlDeclarElementStatement : public IRule
{
public:
	bool detectExpression(std::vector<std::string> &vector)
	{
		if(vector.size() > 2
		   && vector[0] == "<"
		   && vector[1] == "?"
		   && vector[2] == "xml"
		   && vector[vector.size() - 2] == "?"
		   && vector[vector.size() - 1] == ">"
		   && std::find(vector.begin(), vector.end(), "version") != vector.end())
		{
			doActions(vector);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// Action for XmlDeclarElementStatement
class XmlDeclarElementStatementAction : public IAction
{
	void doAction(std::vector<std::string> &vector)
	{
		std::shared_ptr < XmlProcessing::AbstractXmlElement > declarationElement = XmlProcessing::makeXmlDeclarElement();
		Repository *repo = Repository::getInstance();
		repo->pDocElement_ = XmlProcessing::makeDocElement();
		if(repo->pDocElement_ != NULL)
			repo->stack_.push(repo->pDocElement_);
		for(std::vector<std::string>::iterator it = vector.begin(); it < vector.end();)
		{

			while((*it).compare("=") == 0)
			{
				std::string attributeName = *(it - 1);
				std::string attributeValue = *(it + 1);
				attributeValue.erase(std::remove(attributeValue.begin(), attributeValue.end(), '"'), attributeValue.end());
				declarationElement->addAttrib(attributeName, attributeValue);
				break;
			}
			it++;
		}
		repo->stack_.top()->addChild(declarationElement);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect XmlProcsElementStatement
class XmlProcsElementStatement : public IRule
{
public:
	bool detectExpression(std::vector<std::string> &vector)
	{
		if(vector.size() > 0
		   && vector[0] == "<"
		   && vector[1] == "?"
		   && vector[vector.size() - 2] == "?"
		   && vector[vector.size() - 1] == ">"
		   && !(std::find(vector.begin(), vector.end(), "version") != vector.end()))
		{
			doActions(vector);
			return true;
		}
		return false;
	}
};


///////////////////////////////////////////////////////////////
// Action for XmlProcsElementStatement
class XmlProcsElementStatementAction : public IAction
{
public:
	void doAction(std::vector<std::string> &vector)
	{
		Repository *repo = Repository::getInstance();
		for(std::vector<std::string>::iterator it = vector.begin(); it < vector.end();)
		{
			std::string temp = "xml";
			++it; ++it;
			if((*(it + 1)).find("-") != std::string::npos)
			{
				temp = (*it) + (*(it + 1)) + (*(it + 2));
			}
			else
			{
				temp = (*it);
			}
			std::shared_ptr < XmlProcessing::AbstractXmlElement > procInstruction = XmlProcessing::makeProcInstrElement(temp);
			++it;
			while((*it).compare("?") != 0 && (*(it + 1)).compare(">") != 0)
			{
				while((*it).compare("=") == 0)
				{
					std::string attributeName = *(it - 1);
					std::string attributeValue = *(it + 1);
					attributeValue.erase(std::remove(attributeValue.begin(), attributeValue.end(), '"'), attributeValue.end());
					procInstruction->addAttrib(attributeName, attributeValue);
					break;
				}
				it++;
			}

			if(repo->stack_.size() > 0)
				repo->stack_.top()->addChild(procInstruction);
			else
			{
				repo->pDocElement_ = XmlProcessing::makeDocElement();
				if(repo->pDocElement_ != NULL)
				{
					repo->pDocElement_->addChild(procInstruction);
					repo->stack_.push(repo->pDocElement_);
				}
			}
			it++;
			it++;
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect XmlCommentElementStatement
class XmlCommentElementStatement : public IRule
{
public:
	bool detectExpression(std::vector<std::string> &vector)
	{
		if(vector.size() > 0
		   && vector[0] == "<"
		   && vector[1] == "!"
		   && vector[2] == "--"
		   && vector[vector.size() - 2] == "--"
		   && vector[vector.size() - 1] == ">")
		{
			doActions(vector);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action once XmlCommentElementStatement detect
class XmlCommentElementStatementAction : public IAction
{
public:
	void doAction(std::vector<std::string> &vector)
	{
		Repository *repo = Repository::getInstance();
		std::shared_ptr <XmlProcessing::AbstractXmlElement > commentElement = NULL;
		std::vector<std::string>::iterator it = vector.begin();
		it = it + 3;
		std::string comment = "";
		while((*it).compare("--") != 0 && (*(it + 1)).compare(">") != 0)
		{
			comment += (*it) + " ";
			it++;
		}
		// TODO trim() string.
		commentElement = XmlProcessing::makeCommentElement(comment);

		if(commentElement && repo->stack_.size() > 0)
			repo->stack_.top()->addChild(commentElement);
		else if(commentElement)
		{
			repo->pDocElement_ = XmlProcessing::makeDocElement();
			repo->pDocElement_->addChild(commentElement);
			if(repo->pDocElement_ != NULL)
				repo->stack_.push(repo->pDocElement_);
		}
	}
};


///////////////////////////////////////////////////////////////
// rule to detect XmlTaggedElementStatement
class XmlTaggedElementStatement : public IRule
{
private:
	bool isSpecialKeyWord(std::vector<std::string> &v)
	{
		const static std::string keys[]
			= { "?", "!", "--" };
		for(int i = 0; i < 3; ++i)
			if(std::find(v.begin(), v.end(), keys[i]) != v.end())
				return true;
		return false;

	}
public:
	bool detectExpression(std::vector<std::string> &vector)
	{
		if(vector.size() > 0 && !isSpecialKeyWord(vector)
		   && ((vector[0] == "<" && vector[1] == "/")
		   || (vector.size()>= 2 && vector[vector.size() - 2] == "/" && vector[vector.size() - 1] == ">")
		   || (vector.size() >= 2 && vector[0] == "<" && !(vector[1] == "/") && !(vector[vector.size() - 2] == "/") && vector[vector.size() - 1] == ">")))
		{
			doActions(vector);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action takes place once XmlTaggedElementStatement detect
class XmlTaggedElementStatementAction : public IAction
{
	void normalTag(std::vector<std::string> &vector)
	{
		Repository *repo = Repository::getInstance();
		std::vector<std::string>::iterator it = vector.begin();
		if(*it == "<" && (*(it + 1)).compare("/") != 0)
		{
			it++;
			// some tag is here that is going to end or not.
			std::shared_ptr <XmlProcessing::AbstractXmlElement > tagElement = XmlProcessing::makeTaggedElement(*it);
			while((*it).compare("/") != 0 && (*it).compare(">") != 0)
			{
				while((*it).compare("=") == 0)
				{
					std::string attributeName = *(it - 1);
					std::string attributeValue = *(it + 1);
					attributeValue.erase(std::remove(attributeValue.begin(), attributeValue.end(), '"'), attributeValue.end());
					tagElement->addAttrib(attributeName, attributeValue);
					break;
				}
				it++;
			}

			if(tagElement && repo->stack_.size() > 0)
				repo->stack_.top()->addChild(tagElement);
			else if(tagElement)
			{
				repo->pDocElement_ = XmlProcessing::makeDocElement();
				repo->pDocElement_->addChild(tagElement);
				if(repo->pDocElement_ != NULL)
					repo->stack_.push(repo->pDocElement_);
			}
			repo->stack_.push(tagElement);
			if((*it).compare("/") == 0)
			{
				// pop from stack
				repo->stack_.pop();
			}
		}
	}
	void startCloseTag(std::vector<std::string> &vector)
	{
		Repository *repo = Repository::getInstance();
		if(repo->stack_.size() > 0)
			repo->stack_.pop();
	}
public:
	void doAction(std::vector<std::string> &vector)
	{
		if((vector.size() > 1 && vector[0] == "<" && vector[1] == "/")
		   ||
		   (vector.size() == 4
		   && vector[0] == "<"
		   && vector[vector.size() - 2] == "/"
		   && vector[vector.size() - 1] == ">")) // start closing tag
		{
			startCloseTag(vector);
		}
		else if(vector.size() > 0
				&& vector[0] == "<"
				&& !(vector[1] == "/")
				&& vector[vector.size() - 1] == ">") // normal tag
		{
			normalTag(vector);
		}
	}
};

///////////////////////////////////////////////////////////////
// rule to detect XmlTextElementStatement
class XmlTextElementStatement : public IRule
{
private:
	bool isSpecialKeyWord(std::vector<std::string> &v)
	{
		const static std::string keys[]
			= { "<", ">", "/", "?" };
		for(int i = 0; i < 4; ++i)
			if(std::find(v.begin(), v.end(), keys[i]) != v.end())
				return true;
		return false;

	}
public:
	bool detectExpression(std::vector<std::string> &vector)
	{
		if(!isSpecialKeyWord(vector))
		{
			doActions(vector);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action takes place once XmlTextElementStatement detect
class XmlTextElementStatementAction : public IAction
{
public:
	void doAction(std::vector<std::string> &vector)
	{
		Repository *repo = Repository::getInstance();
		std::string textElementValue = "";
		for(size_t i = 0; i < vector.size(); i++)
		{
			if(i == vector.size() - 1)
				textElementValue += vector[i];
			else
				textElementValue += vector[i] + " ";
		}
		std::shared_ptr <XmlProcessing::AbstractXmlElement > textElement = XmlProcessing::makeTextElement(textElementValue);
		if(textElementValue.length() > 0)
		{
			if(repo->stack_.size() > 0)
				repo->stack_.top()->addChild(textElement);
			else
			{
				repo->pDocElement_ = XmlProcessing::makeDocElement();
				if(repo->pDocElement_ != NULL)
				{
					repo->pDocElement_->addChild(textElement);
					repo->stack_.push(repo->pDocElement_);
				}
			}
		}
	}
};
#endif

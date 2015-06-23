#ifndef INTERFACES_H
#define INTERFACES_H

/////////////////////////////////////////////////////////////////////////////
// Interfaces.h- This file holds interface class for rules and actions     //
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
This package provides two classes IRule and IAction which are abstract classes and used by Xml Rules
and Actions classes. Xml Rule and Actions classes implemented these methods
*
* Note : Code Inspired from Dr. Jim Fawcett's parser sample code.
*
* Public Interface:
* ==============================
* Any class can use this abstract classes but derived classes needs to implemented pure virtual methods.
*	void addAction(IAction* pAction); 
*   - This method will be use to add action to any container.
*	void doActions(std::vector<std::string> &tokens);
*   - Called when expression is detect, so apply number of actions on it. Which already registered for particular rule.
*	virtual bool detectExpression(std::vector<std::string> &tokens) = 0;
*   - Used to call detect expression
*	virtual void doAction(std::vector<std::string> &tokens) = 0;
*   - Take action as per xml token inputs.
*
* Maintenance History:
* ============================
* ver 1.0 : 21 March 15
* - first release
*/

#include <vector>
#include <iostream>

///////////////////////////////////////////////////////////////
// abstract base class for actions of expressions
//   - actions are registered with the rule.
class IAction
{
public:
	virtual ~IAction() {}
	virtual void doAction(std::vector<std::string> &tokens) = 0;
};

///////////////////////////////////////////////////////////////
// abstract base class for detections of expressions
//   - rules are registered with the parser for use
class IRule
{
public:
	virtual ~IRule() {}
	void addAction(IAction* pAction);
	void doActions(std::vector<std::string> &tokens);
	virtual bool detectExpression(std::vector<std::string> &tokens) = 0;
protected:
	std::vector<IAction*> actions;
};

#endif
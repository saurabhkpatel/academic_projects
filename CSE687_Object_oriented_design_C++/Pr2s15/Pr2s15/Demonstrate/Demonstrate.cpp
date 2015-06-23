/////////////////////////////////////////////////////////////////////////////
// Demonstrate.cpp - supports functions to demonstrate project #2 requirements//
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project2S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

#include "Demonstarte.h"
#include <iostream>
//----< demonstrate requirement of build tree from xml file or string>---------
void Demonstrate::demonstrateMove()
{
	std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
	std::string test1 = Prologue;
	test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
	XmlProcessing::XmlDocument tempXmlData(test1, XmlProcessing::XmlDocument::string);

	DisplayData::title("Req:4-Demonstrate move constructor for XmlDocument Object");
	XmlProcessing::XmlDocument moveCstrXmlDoc(std::move(tempXmlData));
	if(tempXmlData.getTreeDocumentElement() == NULL)
		std::cout << "  \"tempXmlData\" is XmlDocument object,but its pDocElement is empty now."<< std::endl;
	DisplayData::title(" After move constructor \"moveCstrXmlDoc\" XmlDocument object have \"tempXmlData\" xml document data.",'-');
	std::cout << "    You can check new xml data in \"movecstr.xml\" file." << std::endl;
	DisplayData::showXmlTree(moveCstrXmlDoc.getTreeDocumentElement(), true, "movecstr.xml");
	DisplayData::title("Req:4-Demonstrate move assignment operator for XmlDocument Object");
	XmlProcessing::XmlDocument moveAssignmentXmlDoc(std::move(tempXmlData));
	moveAssignmentXmlDoc = std::move(moveCstrXmlDoc);
	if(moveCstrXmlDoc.getTreeDocumentElement() == NULL)
		std::cout << "  \"moveCstrXmlDoc\" is XmlDocument object,but its pDocElement is empty now." << std::endl;
	DisplayData::title(" After move assignment \"moveAssignmentXmlDoc\" XmlDocument object have \"moveCstrXmlDoc\" xml document data.", '-');
	std::cout << "   You can check new xml data in \"movecstr.xml\" file." << std::endl;
	DisplayData::showXmlTree(moveAssignmentXmlDoc.getTreeDocumentElement(),true ,"moveassignment.xml");
}

//----< demonstrate requirement of build tree from xml file or string>---------
void Demonstrate::demonstrateBuildTree()
{
	DisplayData::title("Req:3 and Req:10-Demonstrate an internal parse tree representation.");
	DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(),true);
}

//----< demonstrate requirement of find attribute >---------
void Demonstrate::demonstrateFindAttribute()
{
	DisplayData::title("Req:5-Demonstrate the capability to find any element based on a unique id attribute.");
	DisplayData::title(" Query to find element which possess \"Company\" = \"Wintellect\" attribute.", '-');
	std::vector<std::shared_ptr <XmlProcessing::AbstractXmlElement >> result = xmldocument->getAttributeByKeyValue("Company", "Wintellect").select();
	DisplayData::showData(result);
	DisplayData::title(" Query to find element which possess \"course\" = \"CSE687\" attribute.", '-');
	std::vector<std::shared_ptr <XmlProcessing::AbstractXmlElement >> result1 = xmldocument->getAttributeByKeyValue("course", "CSE687").select();
	DisplayData::showData(result1);
}

//----< demonstrate requirement of find collections of any element >---------
void Demonstrate::demonstrateFindCollections()
{
	DisplayData::title("Req:6-Demonstrate  the capability to find a collection of elements that have a specified tag.");
	std::vector<std::shared_ptr <XmlProcessing::AbstractXmlElement >> result2 = xmldocument->elements("title").select();
	DisplayData::title(" Query to a collection of elements that have a \"title\" TAG element.", '-');
	DisplayData::showData(result2);

	std::vector<std::shared_ptr <XmlProcessing::AbstractXmlElement >> result4 = xmldocument->elements("title1").select();
	DisplayData::title(" Query to a collection of elements that have a \"title1\" TAG element.", '-');
	DisplayData::showData(result4);
}

//----< demonstrate requirement of add and remove child >---------
void Demonstrate::demonstrateAddRemoveChild()
{
	std::shared_ptr < XmlProcessing::AbstractXmlElement > child = XmlProcessing::makeTaggedElement("child");
	DisplayData::title("Req:7-Demonstrate the capability to add a child element for specified TAG.");
	DisplayData::title(" Query to add <child> </child> element in <LectureNote> TAG.", '-');
	bool result = xmldocument->element("LectureNote").addChild(child);
	if(result)
		DisplayData::title("  Child element added, You can check output.xml file, to verify output", '*');
	else
		DisplayData::title("  Child element not added", '*');
	DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(),true);

	DisplayData::title("Req:7-Demonstrate the capability to remove a child element for specified TAG.");
	DisplayData::title(" Query to remove <child> </child> element in <LectureNote> TAG.", '-');
	result = xmldocument->element("LectureNote").removeChild("child");

	if(result)
		DisplayData::title("  Child element has been removed,You can check output.xml file, to verify output", '*');
	else
		DisplayData::title("  Child element has not been removed.", '*');
	DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(),true);
}

//----< demonstrate requirement of add root element >---------
void Demonstrate::demonstrateRootElement()
{
	DisplayData::title("Req:7-Demonstrate the capability to add a root element to an empty document tree");
	DisplayData::title(" Query to add a root element.", '-');
	std::shared_ptr < XmlProcessing::AbstractXmlElement > rootElement = XmlProcessing::makeTaggedElement("root");
	std::shared_ptr < XmlProcessing::AbstractXmlElement > rootChildElement = XmlProcessing::makeTaggedElement("child");
	rootElement->addChild(rootChildElement);
	rootElement->addAttrib("attr1", "attr1Value");
	rootElement->addAttrib("attr2", "attr2Value");
	std::shared_ptr<XmlProcessing::AbstractXmlElement> rootElementText = XmlProcessing::makeCommentElement("new root node added");
	rootElement->addChild(rootElementText);

	bool addRootElementResult = xmldocument->addRootElement(rootElement);
	if(addRootElementResult)
	{
		DisplayData::title(" New root element added. You can check \"rootElement.xml\" file.", '-');
		DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(), true, "rootElement.xml");
	}
	else
	{
		DisplayData::title(" New root element can not be added, because xml document already have root element", '-');
		DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(), false);
	}
	auto rootCurrentChildren = xmldocument->element().select();
	for(auto &nodechild : rootCurrentChildren[0]->getChildren())
	{
		rootCurrentChildren[0]->removeChild(nodechild);
	}
	DisplayData::title("  Delete root node and other children of xml document object and make pDocElement Empty now.", '-');
	addRootElementResult = xmldocument->addRootElement(rootElement);
	if(addRootElementResult)
	{
		DisplayData::title(" New root element added. You can check \"rootElement.xml\" file.", '-');
		DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(),true,"rootElement.xml");
	}
	else
	{
		DisplayData::title(" New root element can not be added.", '-');
		DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(), false);
	}
}

//----< demonstrate requirement of add and remove attribute>---------
void Demonstrate::demonstrateAddRemoveAttribute()
{
	DisplayData::title("Req:9-Demonstrate add an attribute name-value pair from any element node that supports attributes.");
	DisplayData::title(" Query to add attributes, \"TextType = String\" and \"Attribute2 = value2\"", '-');
	auto result = xmldocument->element("title").addAttribute("TextType", "String");
	result = xmldocument->element("title").addAttribute("Attribute2", "value2");
	if(result)
		DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(),true);
	else
		DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(), false);
	DisplayData::title(" Query to remove attribute, \"Attribute2 = value2\"", '-');
	result = xmldocument->element("title").removeAttribute("Attribute2");
	if(result)
		DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(),true);
	else
		DisplayData::showXmlTree(xmldocument->getTreeDocumentElement(), false);
}

//----< demonstrate requirement to get children and attributes>---------
void Demonstrate::demonstrateChildrenAndAttributes()
{
	DisplayData::title("Req:8-Demonstrate a facility to return a std::vector containing all the name-value attribute pairs attached to input element");
	DisplayData::title(" Query to find attributes pairs of <note> node from <reference> element.", '-');
	auto attributeResults = xmldocument->element("reference").getAttributes("note").selectAttributes();
	DisplayData::showAttributesPair(attributeResults);
	DisplayData::title(" Query to find attributes pairs of <reference> element.", '-');
	attributeResults = xmldocument->element("reference").getAttributes().selectAttributes();
	DisplayData::showAttributesPair(attributeResults);
	DisplayData::title(" Query to find attributes pairs of <random> element, which does not exists actually", '_');
	attributeResults = xmldocument->element().getAttributes("random").selectAttributes();
	DisplayData::showAttributesPair(attributeResults);

	DisplayData::title("Req:8-Demonstrate a facility to  return a std::vector of pointers to all the children of a specified element.");
	DisplayData::title(" Query to find children of <reference> element from <LectureNote> element.", '-');
	auto childrenResults = xmldocument->element("LectureNote").children("reference").select();
	DisplayData::showData(childrenResults);
}

//----< demonstrate requirements using xml file>---------
void Demonstrate::demonstrateFileXml()
{
	demonstrateBuildTree();
	demonstrateFindAttribute();
	demonstrateFindCollections();
	demonstrateAddRemoveChild();
	demonstrateAddRemoveAttribute();
	demonstrateChildrenAndAttributes();
	demonstrateRootElement();
	demonstrateMove();
}

//----< demonstrate requirements using string input>---------
void Demonstrate::demonstrateStringxml()
{
	demonstrateBuildTree();
	demonstrateFindAttribute();
	demonstrateFindCollections();
	demonstrateAddRemoveChild();
	demonstrateAddRemoveAttribute();
	demonstrateChildrenAndAttributes();
	demonstrateRootElement();
	demonstrateMove();
}

#ifdef TEST_DEMONSTRATE
int main(int argc, char* argv[])
{
	std::string fileName = "LectureNote.xml";
	if(argc >= 2)
	{
		fileName = argv[1];
		std::cout << "\n  Processing file " << fileName;
	}
	std::cout << "\n ";

	std::cout << "\n  Testing Xml Document Model Demonstrate class\n "
		<< std::string(30, '=') << std::endl;

	XmlProcessing::XmlDocument* xmlDocument = new XmlProcessing::XmlDocument(fileName, XmlProcessing::XmlDocument::filename);
	if(xmlDocument->getTreeDocumentElement() == NULL)
	{
		std::cout << "\n  Xml Document Object has not been created." << fileName;
		std::cout << "\n ";
		return 1;
	}
	Demonstrate demo(xmlDocument);
	demo.demonstrateFileXml();
	delete xmlDocument;
	return 0;
}
#endif

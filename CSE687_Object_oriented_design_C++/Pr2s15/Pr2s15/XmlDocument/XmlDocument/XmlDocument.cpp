/////////////////////////////////////////////////////////////////////////////
// XmlDocument.h - Supports xml tree build and queries related             //
//                 functions.                                              //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project2S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include "XmlDocument.h"
#include "../../Display/Display.h"
#include "../../BuildXmlTree/BuildXmlTree.h"

using namespace XmlProcessing;

/////////////////////////////////////////////////////////////////
// parse xml file and create tokens
XmlProcessing::XmlDocument::XmlDocument(const std::string& src, sourceType srcType)
{
	try
	{
		TokenParser *pTokenParser = new TokenParser();
		if(pTokenParser->buildXml())
		{
			bool result = false;
			if(srcType == filename)
				result = pTokenParser->attachXmlData(src);
			else
				result = pTokenParser->attachXmlData(src, false);
			if(result)
			{
				pTokenParser->parse();
				pDocElement_ = Repository::getInstance()->pDocElement_;
			}
		}
		
	}
	catch(std::exception ex)
	{
		std::cout << "\n  " << ex.what() << "\n\n";
	}
}

/////////////////////////////////////////////////////////////////
// private function to do DFS search
bool XmlDocument::findByNode(std::shared_ptr<XmlProcessing::AbstractXmlElement> pNode, const std::string& tag, bool returnAll)
{
	if(pNode->value() == tag || tag == "")
	{
		found_.push_back(pNode);
		if(!returnAll)
			return true;
	}
	for(auto pChild : pNode->getChildren())
	{
		if(findByNode(pChild, tag, returnAll))
			return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////
// push input element name in found_ vector
XmlDocument& XmlProcessing::XmlDocument::element(const std::string& tag)
{
	found_.clear();
	if(tag.length() > 0)
		for(auto &nodeChild : pDocElement_->getChildren())
		{
			findByNode(nodeChild, tag, false);
		}
	else
		found_.push_back(pDocElement_);
	return (*this);
}

/////////////////////////////////////////////////////////////////
// add child element.
bool XmlProcessing::XmlDocument::addChild(std::shared_ptr <AbstractXmlElement > sptr)
{
	if(found_.size() > 0)
		return found_[0]->addChild(sptr);
	else
		return pDocElement_->addChild(sptr);
}

/////////////////////////////////////////////////////////////////
// remove child element.
bool XmlProcessing::XmlDocument::removeChild(const std::string& childElement)
{
	if(found_.size() > 0)
	{
		std::shared_ptr<AbstractXmlElement> temp = found_[0];
		for(auto &nodeChild : temp->getChildren())
		{
			if(nodeChild->value() == childElement)
			{
				if(dynamic_cast<TaggedElement*>(nodeChild.get()) != nullptr)
				{
					return temp->removeChild(nodeChild); // TODO remove child later
				}
			}
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////
// get attribute key value pair from xml tree.
XmlDocument& XmlProcessing::XmlDocument::getAttributeByKeyValue(const std::string& attTag, const std::string& attValue)
{
	AttributeSearch attributeSearch(attTag, attValue);
	std::vector<std::shared_ptr<AbstractXmlElement>> temp = pDocElement_->getChildren();
	for(auto &i : temp)
	{
		attributeSearch(i);
	}
	found_ = attributeSearch.getResult();
	return (*this);
}

/////////////////////////////////////////////////////////////////
// add root element if its not exists
bool XmlProcessing::XmlDocument::addRootElement(std::shared_ptr <AbstractXmlElement > sptr)
{
	std::vector<std::shared_ptr<AbstractXmlElement>> temp = pDocElement_->getChildren();
	for(auto &nodeChild : temp)
	{
		if(dynamic_cast<TaggedElement*>(nodeChild.get()) != nullptr)
			return false;
	}
	return pDocElement_->addChild(sptr);
}

/////////////////////////////////////////////////////////////////
// add addtribute to any element.
bool XmlProcessing::XmlDocument::addAttribute(const std::string& key, const 
std::string& value)
{
	if(found_.size() > 0)
	{
		found_[0]->addAttrib(key,value);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////
// remove addtribute to any element.
bool XmlProcessing::XmlDocument::removeAttribute(const std::string& key)
{
	if(found_.size() > 0)
	{
		found_[0]->removeAttrib(key);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////
// getAttributes for provided element, if exists
XmlDocument& XmlProcessing::XmlDocument::getAttributes(const std::string& tag)
{
	if(found_.size() > 0)
	{
		std::shared_ptr<AbstractXmlElement> temp = found_[0];
		if(tag == "")
		{
			for(auto attr : temp->getAttributes())
			{
				foundAttributes_.push_back(attr);
			}
		}
		else
		{
			AttributeSearch attributeSearch(tag);
			std::vector<std::shared_ptr<AbstractXmlElement>> temp = pDocElement_->getChildren();
			for(auto &i : temp)
			{
				attributeSearch(i);
			}
			foundAttributes_ = attributeSearch.getAttributesResult();
		}
	}
	return (*this);
}

/////////////////////////////////////////////////////////////////
// fill collection of elements of provided id/tag name
XmlDocument& XmlProcessing::XmlDocument::elements(const std::string& tag)
{
	found_.clear();
	for(auto &nodeChild : pDocElement_->getChildren())
	{
		findByNode(nodeChild, tag, true);
	}
	return (*this);
}

/////////////////////////////////////////////////////////////////
// fill collection of elements of provided id/tag name
XmlDocument& XmlProcessing::XmlDocument::children(const std::string& tag)
{
	if(found_.size() == 0)
	{
		for(auto &nodeChild : pDocElement_->getChildren())
		{
			findByNode(nodeChild, tag, false);
		}
	}
	if(found_.size() > 0)
	{
		std::shared_ptr<AbstractXmlElement> temp = found_[0];
		found_.clear();
		if(tag == "")
		{
			for(auto &child : temp->getChildren())
			{
				found_.push_back(child);
			}
		}
		else
		{
			for(auto &nodeChild : temp->getChildren())
			{
				if(nodeChild->value() == tag)
				{
					for(auto &child : nodeChild->getChildren())
					{
						found_.push_back(child);
					}
				}
			}
		}
	}
	return (*this);
}

#ifdef TEST_XMLDOCUMENT

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	std::cout << "\n  Processing file " << argv[1];
	std::cout << "\n ";
	std::string fileName = argv[1];
	std::cout << "\n  Testing Xml Document class\n "
		<< std::string(30, '=') << std::endl;
	XmlProcessing::XmlDocument xmlDocument(fileName, XmlProcessing::XmlDocument::filename);
	std::cout << "\n  Testing Xml Document build tree.\n "
		<< std::string(30, '=') << std::endl;
	DisplayData::showXmlTree(xmlDocument.getTreeDocumentElement());

	std::shared_ptr<XmlProcessing::AbstractXmlElement> child = makeTaggedElement("child2");
	child->addAttrib("child2attr1","child2attr1value");
	child->addAttrib("child2attr2", "child2attr2value");
	std::shared_ptr<XmlProcessing::AbstractXmlElement> childcommentText = makeCommentElement("child2 text");
	child->addChild(childcommentText);

	std::cout << "\n  Testing Xml Document addChild \n "
		<< std::string(30, '-') << std::endl;
	xmlDocument.element("parent").addChild(child);
	DisplayData::showXmlTree(xmlDocument.getTreeDocumentElement());

	std::cout << "\n  Testing Xml Document addAttribute \n "
		<< std::string(30, '-') << std::endl;
	xmlDocument.element("child2").addAttribute("child2attr3", "child2attr3value");
	DisplayData::showXmlTree(xmlDocument.getTreeDocumentElement());

	std::cout << "\n  Testing Xml Document removeAttribute \n "
		<< std::string(30, '-') << std::endl;
	xmlDocument.element("child2").removeAttribute("child2attr3");
	DisplayData::showXmlTree(xmlDocument.getTreeDocumentElement());

	std::cout << "\n  Testing Xml Document getAttributeByKeyValue \n "
		<< std::string(30, '-') << std::endl;
	auto temp = xmlDocument.getAttributeByKeyValue("child2attr1", "child2attr1value").select();
	DisplayData::showXmlTree(temp[0]);

	std::cout << "\n  Testing Xml Document getAttributes \n "
		<< std::string(30, '-') << std::endl;
	auto attributes = xmlDocument.element("parent").getAttributes("child2").selectAttributes();
	DisplayData::showAttributesPair(attributes);

	std::cout << "\n  Testing Xml Document childrens \n "
		<< std::string(30, '-') << std::endl;
	auto children2 = xmlDocument.element("parent").children().select();
	DisplayData::showData(children2);

	std::cout << "\n  Testing Xml Document collection of elements. \n "
		<< std::string(30, '-') << std::endl;
	auto children = xmlDocument.elements("child").select();
	DisplayData::showData(children);

	std::cout << "\n  Testing Xml Document removeChild \n "
		<< std::string(30, '-') << std::endl;
	xmlDocument.element("parent").removeChild("child2");
	DisplayData::showXmlTree(xmlDocument.getTreeDocumentElement());

	std::cout << "\n  Testing Xml Document remove node using removeChild Method \n "
		<< std::string(30, '-') << std::endl;
	xmlDocument.element().removeChild("parent");
	DisplayData::showXmlTree(xmlDocument.getTreeDocumentElement());

	std::cout << "\n  Testing Xml Document add root Element \n "
		<< std::string(30, '-') << std::endl;
	std::shared_ptr < XmlProcessing::AbstractXmlElement > rootElement = XmlProcessing::makeTaggedElement("parentRootNode");
	auto children1 = xmlDocument.element().select();
	for(auto &nodechild : children1[0]->getChildren())
	{
		children1[0]->removeChild(nodechild);
	}
	xmlDocument.addRootElement(rootElement);
	DisplayData::showXmlTree(xmlDocument.getTreeDocumentElement());
	return 0;
}
#endif
#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

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

/*
* Package Operations:
* ==============================
* Note : This package construct based on the basic design, which has been provided by Dr. Jim Fawcett.
*
* This package is provides operations for XML Document Model. It uses C++11 constructs,
* most noteably std::shared_ptr. This class provide functions to make queries on XML document
* model. Also provides functionality to create xml build tree.
* Queries provided for below Xml elements.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* Public Interface:
* ==============================
*
* enum sourceType { string, filename }; // enum for input type, file xml OR string xml
* XmlDocument(const std::string& src, sourceType srcType = string); // promo cstr
* XmlDocument& getAttributeByKeyValue(const std::string& attTag, const std::string& attValue); 
*     get attribute based on element id.
* XmlDocument& element(const std::string& tag = "");           
*    found_[0] contains first element (DFS order with tag, when user will not give then it will consider         found_[0] = pDocElement
* XmlDocument& elements(const std::string& tag);          
*    found_ contains all elements with tag
* XmlDocument& children(const std::string& tag = "");  
*    found_ contains sPtrs to descendents of prior found_[0]
* XmlDocument& getAttributes(const std::string& tag = "");
* bool addAttribute(const std::string& key, const std::string& value); 
*    add attribute in input xml tag element
* bool removeAttribute(const std::string& key); 
*    remove attribute from found_[0] shared pointer.
* bool addChild(std::shared_ptr <AbstractXmlElement >); 
*    add child sPtr in found_[0] shared pointer.
* bool removeChild(const std::string& childElement);
*    remove child from found_[0] shared pointer.
* bool XmlProcessing::XmlDocument::addRootElement(std::shared_ptr <AbstractXmlElement > sptr);
*    add root element in xml document if it is not there.
* const std::shared_ptr < AbstractXmlElement >& getTreeDocumentElement()
*    return xml document object
* std::vector<std::shared_ptr < AbstractXmlElement >> select()
*  return reference to found_.  Use std::move(found_) to clear found_
std::vector<std::pair<std::string, std::string>> selectAttributes()
*  return reference to foundAttributes_.  Use std::move(foundAttributes_) to clear foundAttributes_
* AttributeSearch class used as functor to search attribute name and value using DFS pattern
*
* Required Files:
* ==============================
* XmlDocument.cpp, XmlDocument.h, BuildXmlTree.cpp ,BuildXmlTree.h
* Toeknizer.cpp, Tokenizer.h, XmlElement.cpp, XmlElement.h
* XmlElementParts.cpp, XmlElementParts.h
*
* Build Command:
* ==============================
* devenv Pr2s15.sln /rebuild debug
*
* Maintenance History:
* ==============================
*
* Ver 1.0 : 21 March 15
* - first release
*
*/

#include <memory>
#include <iostream>
#include <string>
#include "../XmlElement/XmlElement.h"

namespace XmlProcessing
{
	class XmlDocument
	{
	public:
		enum sourceType { string, filename };
		XmlDocument(const std::string& src, sourceType srcType = string);
		XmlDocument(XmlDocument&& xmlDoc) : pDocElement_(xmlDoc.pDocElement_)
		{
			xmlDoc.pDocElement_ = nullptr;
		}

		XmlDocument& operator=(XmlDocument&& xmldoc)
		{
			if(this == &xmldoc)
			{
				return *this;
			}
			pDocElement_ = xmldoc.pDocElement_;
			found_ = xmldoc.found_;
			foundAttributes_ = xmldoc.foundAttributes_;
			xmldoc.pDocElement_ = nullptr;
			return *this;
		}

		XmlDocument& getAttributeByKeyValue(const std::string& attTag, const std::string& attValue); // get attribute based on element id.
		XmlDocument& element(const std::string& tag = "");           // found_[0] contains first element (DFS order with tag, when user will not give then it will consider found_[0] = pDocElement
		XmlDocument& elements(const std::string& tag);          // found_ contains all elements with tag
		XmlDocument& children(const std::string& tag = "");  // found_ contains sPtrs to descendents of prior found_[0]
		XmlDocument& getAttributes(const std::string& tag = "");

		bool addAttribute(const std::string& key, const std::string& value); // add attribute in input xml tag element
		bool removeAttribute(const std::string& key); // remove attribute from found_[0] shared pointer.
		bool addChild(std::shared_ptr <AbstractXmlElement >); // add child sPtr in input xml tag element
		bool removeChild(const std::string& childElement);
		bool XmlProcessing::XmlDocument::addRootElement(std::shared_ptr <AbstractXmlElement > sptr);
		const std::shared_ptr < AbstractXmlElement >& getTreeDocumentElement()
		{
			return pDocElement_;
		}
		std::vector<std::shared_ptr < AbstractXmlElement >> select()// return reference to found_.  Use std::move(found_) to clear found_
		{
			return std::move(found_);
		}
		std::vector<std::pair<std::string, std::string>> selectAttributes()
		{
			return std::move(foundAttributes_);
		}
	private:
		XmlDocument(const XmlDocument&) = delete;
		XmlDocument & operator=(const XmlDocument&) = delete;

		bool XmlDocument::findByNode(std::shared_ptr<AbstractXmlElement> pNode, const std::string& childElement, bool returnAll); // DFS search
		std::vector<std::shared_ptr < AbstractXmlElement >> found_;  // query results
		std::vector<std::pair<std::string, std::string>> foundAttributes_;
		std::shared_ptr < AbstractXmlElement > pDocElement_; //holds procInstr, comments, XML root, and more comments
	};

	class AttributeSearch
	{
	public:
		AttributeSearch(const std::string& key, const std::string& value, const std::string& tag = "") : attributeKey_(key), attributeValue_(value), tag_(tag) {};
		AttributeSearch(const std::string& tag) : tag_(tag) {};
		void getAttributes(std::shared_ptr<AbstractXmlElement> pNode)
		{
			if(pNode->value() == tag_)
			{
				std::vector<std::pair<std::string, std::string>> attributes = pNode->getAttributes();
				for(auto attribute : attributes)
				{
					attributes_.push_back(attribute);
				}
			}
			for(auto pChild : pNode->getChildren())
			{
				(*this)(pChild);
			}
		}
		/////////////////////////////////////////////////////////////////////
		/// provide the capability to find any element based on a unique id attribute5
		/// for all those elements that possess id attributes.If an element with the specified id 
		/// attribute is found, a pointer to the element node is returned.If no such
		/// element exists a null pointer is returned.
		void getAttributeById(std::shared_ptr<AbstractXmlElement> pNode)
		{
			std::vector<std::pair<std::string, std::string>> attributes = pNode->getAttributes();
			for(auto attribute : attributes)
			{
				if(attribute.first.compare(attributeKey_) == 0 && attribute.second.compare(attributeValue_) == 0)
				{
					found_.push_back(pNode);
					return;
				}
			}
			for(auto pChild : pNode->getChildren())
			{
				(*this)(pChild);
			}
		}
		void operator()(std::shared_ptr<AbstractXmlElement> pNode)
		{
			if(tag_.length() > 0)
				getAttributes(pNode);
			else
				getAttributeById(pNode);
		}
		std::vector<std::shared_ptr<AbstractXmlElement>> getResult()
		{
			return std::move(found_);
		}
		std::vector<std::pair<std::string, std::string>> getAttributesResult()
		{
			return std::move(attributes_);
		}
	private:
		std::vector<std::shared_ptr<AbstractXmlElement>> found_;
		std::vector<std::pair<std::string, std::string>> attributes_;
		std::string attributeKey_;
		std::string attributeValue_;
		std::string tag_;
	};
}
#endif

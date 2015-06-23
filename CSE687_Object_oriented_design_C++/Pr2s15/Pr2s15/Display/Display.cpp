#include "display.h"

/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Supports display functions.                               //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project2S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

#ifdef TEST_DISPLAY
int main(int argc, char* argv[])
{
	DisplayData::title("Testing Display Package", '=');
	std::shared_ptr < XmlProcessing::AbstractXmlElement > root = XmlProcessing::makeTaggedElement("root");
	root->addChild(XmlProcessing::makeTextElement("this is a test"));
	std::shared_ptr < XmlProcessing::AbstractXmlElement > child = XmlProcessing::makeTaggedElement("child");
	child->addChild(XmlProcessing::makeTextElement("this is another test"));
	child->addAttrib("first", "test");
	root->addChild(child);
	std::shared_ptr < XmlProcessing::AbstractXmlElement > docEl = makeDocElement(root);

	DisplayData::title(" show xml tree ", '-');
	DisplayData::showXmlTree(docEl);

	std::vector<std::shared_ptr <XmlProcessing::AbstractXmlElement >> result;
	result.push_back(root);
	result.push_back(child);
	DisplayData::title("show xml elements ", '-');
	DisplayData::showData(result);

	std::vector<std::pair<std::string, std::string>> attributePairs;
	std::pair < std::string, std::string> pair1 = std::make_pair("att1", "val1");
	std::pair < std::string, std::string> pair2 = std::make_pair("att2", "val2");
	std::pair < std::string, std::string> pair3 = std::make_pair("att3", "val3");
	attributePairs.push_back(pair1);
	attributePairs.push_back(pair2);
	attributePairs.push_back(pair3);
	DisplayData::title("show xml attribute pairs ", '-');
	DisplayData::showAttributesPair(attributePairs);

	std::cout << "\n\n";
	return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Display.cpp - Supports display functions.                               //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project3S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
/////////////////////////////////////////////////////////////////////////////

#include "Display.h"

#ifdef TEST_DISPLAY
int main()
{

	Display::title("Demonstarte Display class");
	Display::displaySendInformation("File.txt","localhost",9080);
	Display::displaySendInformation("test string", "localhost", 9080,false);
	Display::displaySendInformation("test buffer data", "localhost", 9080, false,false);
	Display::displayReceviedFile("File.txt");
	Display::displayErrorLog("Opps Its Error !! ");
	Display::show("Its normal Message");

}
#endif
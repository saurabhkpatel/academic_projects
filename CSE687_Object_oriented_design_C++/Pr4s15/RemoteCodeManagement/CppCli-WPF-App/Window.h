#ifndef WINDOW_H
#define WINDOW_H
/////////////////////////////////////////////////////////////////////////////
// Window.h -  C++\CLI implementation of WPF Application for project #4    //
//																		   //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project4S15                //
// Modified by: Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
// Author :     Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding Channel. Channel hosts a client which having one sender
*  and receiver and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The WFP application can post a message to the Channel's client's sending queue. It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  to update GUI of WPF application. So this Demo simulates operations you will need to execute for Project #4.
*
* Public Interface:
* ==============================
*  WPFCppCliDemo()
*  - Class constructor, to initialize GUI
*
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, HttpMessage.cpp and Channel DLL
*
*  Build Command:
*  --------------
* devenv RemoteCodeManagement.sln /rebuild debug
*  - this builds C++\CLI client application.
*
*  Maintenance History:
*  --------------------
*  ver 3.0 : 28 Apr 2015
*  - completed project 4 related GUI
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../Channel/Channel.h"
#include <iostream>
#include<chrono>

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
	public:
		WPFCppCliDemo(array<System::String^>^ args);
		~WPFCppCliDemo();
	private:
		bool ParseCommandLineArgs(array<System::String^>^ args);
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);

		// GUI methods
		void updateListBox(array<String^>^ args);
		void updateStatusBox(String^ msg);
		void updateStatusTimeBox(String^ msg);
		void setUpStatusBar();
		void setUpTabControl();
		void setUpFileUploadView();
		void setFileUploadPanel();
		void setFileUploadButtons();
		void setFileUploadComBoxes();
		void setUpFileSearchView();
		void setFileSearchPanel();
		void setFileSearchListBox();
		void setFileSearchServerLabels();
		void setFileSearchPanels();
		void setFileSearchUpperButtonProperties();
		void setFileSearchTextBoxProperties();
		void setFileSearchButtonsProperties();

		// operations methods
		void getMessage();
		array<String^>^ getFileNameWithDetails(Message& msg);
		void clear(Object^ sender, RoutedEventArgs^ args);
		void browseForFiles(Object^ sender, RoutedEventArgs^ args);
		void uploadFileClick(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
		void sendSearchFilesCommand(Object^ obj, RoutedEventArgs^ args);
		void sendDownloadFileCommand(Object^ obj, RoutedEventArgs^ args);
		void searchText(Object^ obj, RoutedEventArgs^ args);


		// MockChannel references
		IChannel* pChann_;

		// Controls for Window

		// support docking statusbar at bottom
		DockPanel^ hDockPanel = gcnew DockPanel();
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hUploadFileTab = gcnew TabItem();
		TabItem^ hFileSearchTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		StatusBarItem^ hStatusBarItemTime = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();
		TextBlock^ hStatusTime = gcnew TextBlock();

		//Controls for File Upload
		Grid^ hUploadFileGrid = gcnew Grid();
		Button^ hBrowseButton = gcnew Button();
		Button^ hUploadFileButton = gcnew Button();
		StackPanel^ hStackPanelMain = gcnew StackPanel();
		StackPanel^ hStackPanelServer1 = gcnew StackPanel();
		StackPanel^ hStackPanelUploadButtons = gcnew StackPanel();
		StackPanel^ hStackPanelServer2 = gcnew StackPanel();
		CheckBox^ checkBox1 = gcnew CheckBox();
		CheckBox^ checkBox2 = gcnew CheckBox();
		Label^ label1 = gcnew Label();
		Label^ label2 = gcnew Label();
		Label^ label3 = gcnew Label();
		Label^ label4 = gcnew Label();
		String^ selectedFile = "";
		Forms::OpenFileDialog^ hFileBrowserDialog = gcnew Forms::OpenFileDialog();
		ComboBox^ comboBoxServer1 = gcnew ComboBox();
		ComboBox^ comboBoxServer2 = gcnew ComboBox();

		//Controls for File Search
		Grid^ hFileSearchGrid = gcnew Grid();
		Button^ hTextSearchButton = gcnew Button();
		Button^ hFileSearchButton = gcnew Button();
		Button^ hFsDownloadButton = gcnew Button();
		Button^ hFsClearButton = gcnew Button();
		TextBox^ hTextBoxFileUpPatt = gcnew TextBox();
		TextBox^ hTextBoxFileTextSearch = gcnew TextBox();
		WrapPanel^ hStackPanelFileSearch = gcnew WrapPanel();
		ScrollViewer^ hScrollViewerFileSearch = gcnew ScrollViewer();
		Label^ fsLabel1 = gcnew Label();
		Label^ fsLabel2 = gcnew Label();
		Label^ fsLabel11 = gcnew Label();
		Label^ fsLabel22 = gcnew Label();
		Label^ fsLabel3 = gcnew Label();
		Label^ fsLabel4 = gcnew Label();
		ListBox^ hFsListBox = gcnew ListBox();
		ComboBox^ comboBox1 = gcnew ComboBox();

		CheckBox^ checkBoxFs1 = gcnew CheckBox();
		CheckBox^ checkBoxFs2 = gcnew CheckBox();

		ComboBox^ comboBoxFsServer1 = gcnew ComboBox();
		ComboBox^ comboBoxFsServer2 = gcnew ComboBox();

		StackPanel^ hStackPanelFsServer1 = gcnew StackPanel();
		StackPanel^ hStackPanelFsServer2 = gcnew StackPanel();
		StackPanel^ hStackPanelFsServers = gcnew StackPanel();
		StackPanel^ hStackPanelFsPatterns = gcnew StackPanel();
		StackPanel^ hStackPanelFsTextSearch = gcnew StackPanel();
		StackPanel^ hStackPanelFsButtons = gcnew StackPanel();
		StackPanel^ hStackPanelFsControls = gcnew StackPanel();
		StackPanel^ hStackPanelFsControlsMain = gcnew StackPanel();
		StackPanel^ hStackPanelFsRequestButtons = gcnew StackPanel();
		StackPanel^ hStackPanelFsMain = gcnew StackPanel();

		Grid^ hFileListGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		Button^ hFolderBrowseButton = gcnew Button();
		Grid^ hGrid2 = gcnew Grid();

		// receive thread
		Thread^ recvThread;

		// server and client related details.
		String^ clientdetails;
		String^ server1details;
		String^ server2details;
		String^ server2Address;
		String^ server2Port;
		String^ server1Address;
		String^ server1Port;
		String^ clientAddress;
		String^ clientPort;
	};

	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
}


#endif

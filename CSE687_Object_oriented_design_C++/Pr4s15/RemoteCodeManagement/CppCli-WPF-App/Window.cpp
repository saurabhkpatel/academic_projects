/////////////////////////////////////////////////////////////////////////////
// Window.cpp -  C++\CLI implementation of WPF Application for project #4  //
//																		   //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell Inspiron 14z, Core i5, Windows 8.1                    //
// Application: CSE687 - Object Oriented Design Project4S15                //
// Author:      Saurabh Patel, MSCE Current Graduate Student,              //
//              Syracuse Univesity                                         //
//              skpatel@syr.edu                                            //
// Source :     Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
/////////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/

#include "Window.h"
using namespace CppCliWindows;

// <----- WPF Application constructor initalization --- >
WPFCppCliDemo::WPFCppCliDemo(array<System::String^>^ args)
{
	if(!ParseCommandLineArgs(args))
		return;
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pChann_ = pObjFact->createChannel(toStdString(clientAddress),stoi(toStdString(clientPort)));
	pChann_->start();
	delete pObjFact;
	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	// set event handlers
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFiles);
	hUploadFileButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadFileClick);
	hTextSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchText);
	hFileSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendSearchFilesCommand);
	hFsDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendDownloadFileCommand);
	hFsClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	// set Window properties
	this->Title = "WPF C++/CLI Project#4 Application";
	this->Width = 1200;
	this->Height = 700;
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl();
	setUpStatusBar();
	setUpFileUploadView();
	setUpFileSearchView();
}

// <---------- WPF Application dsctr ---------->
WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
}

// <----- setStatus Bar property ---------->
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatusBar->Items->Add(hStatusBarItemTime);
	
	hStatus->Text = "very important messages will appear here";
	hStatus->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hStatus->FontWeight = FontWeights::Bold;
	hStatus->FontSize = 12;
	hStatusBarItem->Content = hStatus;

	hStatusTime->Text = "";
	hStatusTime->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hStatusTime->FontSize = 12;
	hStatusBarItemTime->Content = hStatusTime;

	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

// <----  setup tab controls -->
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hUploadFileTab->Header = "Upload Files";
	hFileSearchTab->Header = "File Search/Text Search/Download";
	hTabControl->Items->Add(hUploadFileTab);
	hTabControl->Items->Add(hFileSearchTab);
}

// <----  setup FileUpload GUI View -->
void WPFCppCliDemo::setUpFileUploadView()
{
	Console::Write("\n  setting up Connection view");
	hUploadFileGrid->Margin = Thickness(20);
	hUploadFileTab->Content = hUploadFileGrid;
	setFileUploadPanel();
}

// <----   setup FileUpload comboxes -->
void WPFCppCliDemo::setFileUploadComBoxes()
{
	comboBoxServer1->Height = 25;
	comboBoxServer1->Width = 90;
	comboBoxServer2->Height = 25;
	comboBoxServer2->Width = 90;
	comboBoxServer1->Items->Add("root");
	comboBoxServer1->Items->Add("Java");
	comboBoxServer1->Items->Add("C++");
	comboBoxServer1->Items->Add("C#");
	comboBoxServer1->SelectedIndex = 0;
	comboBoxServer1->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	comboBoxServer2->Items->Add("root");
	comboBoxServer2->Items->Add("Java");
	comboBoxServer2->Items->Add("C++");
	comboBoxServer2->Items->Add("C#");
	comboBoxServer2->SelectedIndex = 0;
	comboBoxServer2->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	label1->Content = "Server 1";
	label1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	label1->FontWeight = FontWeights::Bold;
	label1->FontSize = 16;
	label1->Height = 40;
	label1->Width = 100;
	label3->Content = server1details;
	label3->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	label3->FontSize = 16;
	label3->Height = 40;
	label3->Width = 200;
	label2->Content = "Server 2";
	label2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	label2->FontWeight = FontWeights::Bold;
	label2->FontSize = 16;
	label2->Height = 40;
	label2->Width = 100;
	label4->Content = server2details;
	label4->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	label4->FontSize = 16;
	label4->Height = 40;
	label4->Width = 200;
}

// <----set File Upload GUI Buttons  -->
void WPFCppCliDemo::setFileUploadButtons()
{
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 50;

	Border^ hBorderBrowseButton = gcnew Border();
	hBorderBrowseButton->Width = 120;
	hBorderBrowseButton->Height = 30;
	hBorderBrowseButton->BorderThickness = Thickness(1);
	hBorderBrowseButton->BorderBrush = Brushes::Black;
	hBrowseButton->Content = "Browse";
	hBorderBrowseButton->Child = hBrowseButton;

	Border^ hBorderUploadFileButton = gcnew Border();
	hBorderUploadFileButton->Width = 120;
	hBorderUploadFileButton->Height = 30;
	hBorderUploadFileButton->BorderThickness = Thickness(1);
	hBorderUploadFileButton->BorderBrush = Brushes::Black;
	hUploadFileButton->Content = "Upload File";
	hBorderUploadFileButton->Child = hUploadFileButton;

	hStackPanelUploadButtons->Orientation = Orientation::Horizontal;
	hStackPanelUploadButtons->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanelUploadButtons->Children->Add(hBorderBrowseButton);
	hStackPanelUploadButtons->Children->Add(hSpacer);
	hStackPanelUploadButtons->Children->Add(hBorderUploadFileButton);
}

// <---- set file upload view -->
void WPFCppCliDemo::setFileUploadPanel()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(400);
	hUploadFileGrid->RowDefinitions->Add(hRow2Def);

	Border^ panelBorder = gcnew Border();
	panelBorder->Width = 800;
	panelBorder->Height = 400;
	panelBorder->BorderThickness = Thickness(1);
	panelBorder->BorderBrush = Brushes::Black;

	setFileUploadButtons();
	setFileUploadComBoxes();

	checkBox1->Width = 30;
	checkBox1->Height = 30;
	checkBox2->Height = 30;
	checkBox2->Width = 20;
	checkBox1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	checkBox2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;

	hStackPanelServer1->Orientation = Orientation::Horizontal;
	hStackPanelServer1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanelServer1->Children->Add(checkBox1);
	hStackPanelServer1->Children->Add(label1);
	hStackPanelServer1->Children->Add(label3);
	hStackPanelServer1->Children->Add(comboBoxServer1);
	hStackPanelServer2->Orientation = Orientation::Horizontal;
	hStackPanelServer2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanelServer2->Children->Add(checkBox2);
	hStackPanelServer2->Children->Add(label2);
	hStackPanelServer2->Children->Add(label4);
	hStackPanelServer2->Children->Add(comboBoxServer2);
	TextBlock^ hBlankSpacer = gcnew TextBlock();
	hBlankSpacer->Width = 350;
	hStackPanelMain->Orientation = Orientation::Vertical;
	hStackPanelMain->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hStackPanelMain->Children->Add(hStackPanelServer1);
	hStackPanelMain->Children->Add(hStackPanelServer2);
	hStackPanelMain->Children->Add(hBlankSpacer);
	hStackPanelMain->Children->Add(hStackPanelUploadButtons);

	panelBorder->Child = hStackPanelMain;
	hUploadFileGrid->SetRow(panelBorder, 1);
	hUploadFileGrid->Children->Add(panelBorder);
}

// <---- setup File/Text Search Views  -->
void WPFCppCliDemo::setUpFileSearchView()
{
	Console::Write("\n  setting up file search view");
	hFileSearchGrid->Margin = Thickness(20);
	hFileSearchTab->Content = hFileSearchGrid;
	setFileSearchPanel();
}


// <---- setup File/Text Search GUI , Listbox control. -->
void WPFCppCliDemo::setFileSearchListBox()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hFileSearchGrid->RowDefinitions->Add(hRow2Def);

	Border^ hFileSearchResults = gcnew Border();
	hFileSearchResults->BorderThickness = Thickness(1);
	hFileSearchResults->BorderBrush = Brushes::Black;
	hFileSearchResults->Width = 800;
	hFileSearchResults->Height = 300;
	hFileSearchResults->Child = hFsListBox;
	hFileSearchGrid->SetRow(hFileSearchResults, 1);
	hFileSearchGrid->Children->Add(hFileSearchResults);
}

// <----  set File search server label panels -->
void WPFCppCliDemo::setFileSearchServerLabels()
{
	fsLabel1->Content = "Server 1";
	fsLabel1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabel1->FontWeight = FontWeights::Bold;
	fsLabel1->FontSize = 16;
	fsLabel1->Height = 40;
	fsLabel1->Width = 90;
	fsLabel11->Content = server1details;
	fsLabel11->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabel11->FontSize = 16;
	fsLabel11->Height = 40;
	fsLabel11->Width = 130;
	fsLabel2->Content = "Server 2";
	fsLabel2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabel2->FontWeight = FontWeights::Bold;
	fsLabel2->FontSize = 16;
	fsLabel2->Height = 40;
	fsLabel2->Width = 90;
	fsLabel22->Content = server2details;
	fsLabel22->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabel22->FontSize = 16;
	fsLabel22->Height = 40;
	fsLabel22->Width = 130;
	checkBoxFs1->Width = 30;
	checkBoxFs1->Height = 30;
	checkBoxFs2->Height = 30;
	checkBoxFs2->Width = 20;
	checkBoxFs1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	checkBoxFs2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	comboBoxFsServer1->Height = 25;
	comboBoxFsServer1->Width = 90;
	comboBoxFsServer1->Items->Add("root");	comboBoxFsServer1->Items->Add("Java");comboBoxFsServer1->Items->Add("C++");
	comboBoxFsServer1->Items->Add("C#");
	comboBoxFsServer1->SelectedIndex = 0;
	comboBoxFsServer1->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	comboBoxFsServer2->Height = 25;
	comboBoxFsServer2->Width = 90;
	comboBoxFsServer2->Items->Add("root");comboBoxFsServer2->Items->Add("Java");
	comboBoxFsServer2->Items->Add("C++");comboBoxFsServer2->Items->Add("C#");
	comboBoxFsServer2->SelectedIndex = 0;
	comboBoxFsServer2->VerticalAlignment = System::Windows::VerticalAlignment::Center;
}

// <----  set File search panels and add children -->
void WPFCppCliDemo::setFileSearchPanels()
{
	hStackPanelFsServer1->Orientation = Orientation::Horizontal;
	hStackPanelFsServer1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanelFsServer1->Children->Add(checkBoxFs1);
	hStackPanelFsServer1->Children->Add(fsLabel1);
	hStackPanelFsServer1->Children->Add(fsLabel11);
	hStackPanelFsServer1->Children->Add(comboBoxFsServer1);


	hStackPanelFsServer2->Orientation = Orientation::Horizontal;
	hStackPanelFsServer2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanelFsServer2->Children->Add(checkBoxFs2);
	hStackPanelFsServer2->Children->Add(fsLabel2);
	hStackPanelFsServer2->Children->Add(fsLabel22);
	hStackPanelFsServer2->Children->Add(comboBoxFsServer2);

	hStackPanelFsServers->Orientation = Orientation::Vertical;
	hStackPanelFsServers->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hStackPanelFsServers->Children->Add(hStackPanelFsServer1);
	hStackPanelFsServers->Children->Add(hStackPanelFsServer2);
}

// <----  set File search upper button properties ---->
void WPFCppCliDemo::setFileSearchUpperButtonProperties()
{
	hTextSearchButton->Content = "Text Search";
	Border^ hTextSearchBorder = gcnew Border();
	hTextSearchBorder->Width = 100;
	hTextSearchBorder->Height = 30;
	hTextSearchBorder->BorderThickness = Thickness(1);
	hTextSearchBorder->BorderBrush = Brushes::Black;
	hTextSearchBorder->Child = hTextSearchButton;

	hFileSearchButton->Content = "File Search";
	Border^ hFileSearchBorder = gcnew Border();
	hFileSearchBorder->Width = 100;
	hFileSearchBorder->Height = 30;
	hFileSearchBorder->BorderThickness = Thickness(1);
	hFileSearchBorder->BorderBrush = Brushes::Black;
	hFileSearchBorder->Child = hFileSearchButton;

	hStackPanelFsButtons->Orientation = Orientation::Horizontal;
	hStackPanelFsButtons->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	//hStackPanelFsButtons->Children->Add(comboBoxFsServer1);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 50;
	hStackPanelFsButtons->Children->Add(hSpacer);
	hStackPanelFsButtons->Children->Add(hTextSearchBorder);
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 50;
	hStackPanelFsButtons->Children->Add(hSpacer3);
	hStackPanelFsButtons->Children->Add(hFileSearchBorder);
}

// <----  set File search text box properties ---->
void WPFCppCliDemo::setFileSearchTextBoxProperties()
{
	fsLabel3->Content = "File Pattern(Space seperated) : ";
	fsLabel3->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabel3->FontSize = 14;
	fsLabel3->Height = 40;
	fsLabel3->Width = 200;
	hTextBoxFileUpPatt->Width = 170;
	hTextBoxFileUpPatt->Height = 25;
	hTextBoxFileUpPatt->FontSize = 14;
	hTextBoxFileUpPatt->BorderBrush = Brushes::Black;
	hTextBoxFileUpPatt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBoxFileUpPatt->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	fsLabel4->Content = "Text search : ";
	fsLabel4->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabel4->FontSize = 14;
	fsLabel4->Height = 40;
	fsLabel4->Width = 150;
	hTextBoxFileTextSearch->Width = 170;
	hTextBoxFileTextSearch->Height = 25;
	hTextBoxFileTextSearch->FontSize = 14;
	hTextBoxFileTextSearch->BorderBrush = Brushes::Black;
	hTextBoxFileTextSearch->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBoxFileTextSearch->HorizontalAlignment = System::Windows::HorizontalAlignment::Right;
	hStackPanelFsPatterns->Orientation = Orientation::Horizontal;
	hStackPanelFsPatterns->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanelFsPatterns->Children->Add(fsLabel3);
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10;
	hStackPanelFsPatterns->Children->Add(hSpacer1);
	hStackPanelFsPatterns->Children->Add(hTextBoxFileUpPatt);
	hStackPanelFsTextSearch->Orientation = Orientation::Horizontal;
	hStackPanelFsTextSearch->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanelFsTextSearch->Children->Add(fsLabel4);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	hStackPanelFsTextSearch->Children->Add(hSpacer2);
	hStackPanelFsTextSearch->Children->Add(hTextBoxFileTextSearch);
	setFileSearchUpperButtonProperties();
	hStackPanelFsControls->Orientation = Orientation::Vertical;
	hStackPanelFsControls->VerticalAlignment = System::Windows::VerticalAlignment::Top;
	hStackPanelFsControls->Children->Add(hStackPanelFsPatterns);
	hStackPanelFsControls->Children->Add(hStackPanelFsTextSearch);
	hStackPanelFsControls->Children->Add(hStackPanelFsButtons);
}

// <---- setup File/Text Search GUI panels. -- >
void WPFCppCliDemo::setFileSearchPanel()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(150);
	hFileSearchGrid->RowDefinitions->Add(hRow1Def);

	Border^ panelBorder = gcnew Border();
	panelBorder->Width = 800;
	panelBorder->Height = 150;
	panelBorder->BorderThickness = Thickness(1);
	panelBorder->BorderBrush = Brushes::Black;

	setFileSearchListBox();
	setFileSearchServerLabels();
	setFileSearchPanels();
	setFileSearchTextBoxProperties();


	hStackPanelFsControlsMain->Orientation = Orientation::Horizontal;
	hStackPanelFsControlsMain->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hStackPanelFsControlsMain->Children->Add(hStackPanelFsServers);
	hStackPanelFsControlsMain->Children->Add(hStackPanelFsControls);

	panelBorder->Child = hStackPanelFsControlsMain;
	hFileSearchGrid->SetRow(panelBorder, 0);
	hFileSearchGrid->Children->Add(panelBorder);

	setFileSearchButtonsProperties();
}

// <----  set File search below panel buttons properties ---->
void WPFCppCliDemo::setFileSearchButtonsProperties()
{
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(75);
	hFileSearchGrid->RowDefinitions->Add(hRow4Def);

	Border^ hBorder4 = gcnew Border();
	hFsDownloadButton->Content = "Download";
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hFsDownloadButton;

	Border^ hBorder5 = gcnew Border();
	hFsClearButton->Content = "Clear";
	hBorder5->Width = 120;
	hBorder5->Height = 30;
	hBorder5->BorderThickness = Thickness(1);
	hBorder5->BorderBrush = Brushes::Black;
	hBorder5->Child = hFsClearButton;

	hStackPanelFsRequestButtons->Children->Add(hBorder4);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	hStackPanelFsRequestButtons->Children->Add(hSpacer);
	hStackPanelFsRequestButtons->Children->Add(hBorder5);
	hStackPanelFsRequestButtons->Orientation = Orientation::Horizontal;
	hStackPanelFsRequestButtons->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;

	hFileSearchGrid->SetRow(hStackPanelFsRequestButtons, 2);
	hFileSearchGrid->Children->Add(hStackPanelFsRequestButtons);
}

// convert CLI string to std nativ string.
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for(int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

// convert std::strint to system^ string
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for(size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}


// <--  File upload brwose button click -->
void WPFCppCliDemo::browseForFiles(Object^ sender, RoutedEventArgs^ args)
{
	System::Windows::Forms::DialogResult result;
	result = hFileBrowserDialog->ShowDialog();
	if(result == System::Windows::Forms::DialogResult::OK)
	{
		selectedFile = hFileBrowserDialog->FileName;
	}
}

// <--  File upload upload button click -->
void WPFCppCliDemo::uploadFileClick(Object^ sender, RoutedEventArgs^ args)
{
	if(selectedFile->Length > 0)
	{
		bool selected = false;
		Message fileUploadMessage;
		fileUploadMessage.commandType = UPLOAD;
		fileUploadMessage.fileName = toStdString(selectedFile);
		fileUploadMessage.senderPort = stoi(toStdString(clientPort));
		fileUploadMessage.sourceAddress = toStdString(clientAddress);
		if(checkBox1->IsChecked)
		{
			selected = true;
			fileUploadMessage.catagoryType = toStdString(comboBoxServer1->SelectedValue->ToString());
			fileUploadMessage.destAdd = "localhost";
			fileUploadMessage.destPort = 9085;
			pChann_->postMessage(fileUploadMessage);
			Console::Write("\n  File upload message sent to server1.");
			hStatus->Text = "File Upload message sent to Server 1.";
		}
		if(checkBox2->IsChecked)
		{
			selected = true;
			fileUploadMessage.catagoryType = toStdString(comboBoxServer2->SelectedValue->ToString());
			fileUploadMessage.destAdd = "localhost";
			fileUploadMessage.destPort = 9086;
			pChann_->postMessage(fileUploadMessage);
			Console::Write("\n  File Upload message sent to server 2.");
			hStatus->Text = "File upload message sent to server 2.";
		}
		if(!selected)
		{
			Console::Write("\n  Please select atleast one server.");
			hStatus->Text = "Please select atleast one server.";
		}

	}
	else
	{
		Console::Write("\n  File is not selected.");
		hStatus->Text = "File is not selected, Message not sent.";
	}
}

// <--  search text from remote server files. -->
void WPFCppCliDemo::searchText(Object^ obj, RoutedEventArgs^ args)
{
	bool result = false;
	String ^pattern = hTextBoxFileUpPatt->Text;
	String ^textSearch = hTextBoxFileTextSearch->Text;
	if(pattern->Length == 0)
		pattern = "*.*";
	if(textSearch->Length == 0)
	{
		Console::Write("\n  Please enter text for search.");
		hStatus->Text = "Please enter text for search.";
		return;
	}
	Message textSearchMessage;
	textSearchMessage.commandType = TEXTSEARCH;
	textSearchMessage.senderPort = stoi(toStdString(clientPort));
	textSearchMessage.sourceAddress = toStdString(clientAddress);
	textSearchMessage.fileName = "NA";
	std::string patternString = toStdString(pattern);
	textSearchMessage.patterns = patternString;
	textSearchMessage.textSearch = toStdString(textSearch);

	if(checkBoxFs1->IsChecked)
	{
		result = true;
		textSearchMessage.destAdd = toStdString(server1Address);
		textSearchMessage.destPort = stoi(toStdString(server1Port));
		textSearchMessage.catagoryType = toStdString(comboBoxFsServer1->SelectedValue->ToString());
		pChann_->postMessage(textSearchMessage);
	}
	if(checkBoxFs2->IsChecked)
	{
		result = true;
		textSearchMessage.destAdd = toStdString(server2Address);
		textSearchMessage.destPort = stoi(toStdString(server2Port));
		textSearchMessage.catagoryType = toStdString(comboBoxFsServer2->SelectedValue->ToString());
		pChann_->postMessage(textSearchMessage);
	}
	if(!result)
	{
		Console::Write("\n  Please select atleast one server.");
		hStatus->Text = "Please select atleast one server.";
	}
	else
	{
		start = std::chrono::steady_clock::now();
		Console::Write("\n sent message for text search");
		hStatus->Text = "Sent message for Text Search";
	}
}

// <--  search files from remote server. -->
void WPFCppCliDemo::sendSearchFilesCommand(Object^ obj, RoutedEventArgs^ args)
{
	bool result = false;
	String ^pattern = hTextBoxFileUpPatt->Text;
	if(pattern->Length == 0)
	{
		pattern = "*.*";
	}
	Message getFilesMessage;
	getFilesMessage.commandType = GETFILES;
	getFilesMessage.senderPort = stoi(toStdString(clientPort));
	getFilesMessage.sourceAddress = toStdString(clientAddress);

	getFilesMessage.fileName = "NA";
	getFilesMessage.patterns = toStdString(pattern);

	if(checkBoxFs1->IsChecked)
	{
		result = true;
		getFilesMessage.destAdd = toStdString(server1Address);
		getFilesMessage.destPort = stoi(toStdString(server1Port));
		getFilesMessage.catagoryType = toStdString(comboBoxFsServer1->SelectedValue->ToString());
		pChann_->postMessage(getFilesMessage);
	}
	if(checkBoxFs2->IsChecked)
	{
		result = true;
		getFilesMessage.destAdd = toStdString(server2Address);
		getFilesMessage.destPort = stoi(toStdString(server2Port));
		getFilesMessage.catagoryType = toStdString(comboBoxFsServer2->SelectedValue->ToString());
		pChann_->postMessage(getFilesMessage);
	}
	if(!result)
	{
		Console::Write("\n  Please select atleast one server.");
		hStatus->Text = "Please select atleast one server.";
	}
	else
	{
		start = std::chrono::steady_clock::now();
		Console::Write("\n sent message for File Search");
		hStatus->Text = "File search message sent";
	}
}

// <--  send download command for selected file. -->
void WPFCppCliDemo::sendDownloadFileCommand(Object^ obj, RoutedEventArgs^ args)
{
	Message downloadMessage;
	downloadMessage.commandType = DOWNLOAD;
	downloadMessage.sourceAddress = toStdString(clientAddress);
	downloadMessage.senderPort = stoi(toStdString(clientPort));

	if(hFsListBox->SelectedValue)
	{
		String^ fileNameWithServer = hFsListBox->SelectedItem->ToString();
		array<System::String^>^ temp1 = fileNameWithServer->Split(' ');
		if(temp1->Length == 2)
		{
			array<System::String^>^ temp2 = temp1[0]->Split(':');
			downloadMessage.destAdd = toStdString(temp2[0]);
			downloadMessage.destPort = stoi(toStdString(temp2[1]));
			downloadMessage.catagoryType = toStdString(comboBoxFsServer1->SelectedValue->ToString());
			downloadMessage.fileName = toStdString(temp1[1]);
			pChann_->postMessage(downloadMessage);
			Console::Write("\n File download message sent");
			hStatus->Text = "File download message sent";
		}
		else
		{
			Console::Write("\n File download message not sent");
			hStatus->Text = "Error in Sent message";
			return;
		}
	}
	else
	{
		Console::Write("\n  Please select proper value from list.");
		hStatus->Text = "Please select proper value from list.";
	}
}

// <-- Update List box of file search. -->
void WPFCppCliDemo::updateListBox(array<String^>^ args)
{
	for each(String^ arg in args)
		hFsListBox->Items->Add(arg);
	hStatus->Text = "File list updated.";
}

// <-- Update status message in application. -->
void WPFCppCliDemo::updateStatusBox(String^ msg)
{
	hStatus->Text = msg;
	hStatusTime->Text = "";
}

// <-- Update time status message in application. -->
void WPFCppCliDemo::updateStatusTimeBox(String^ msg)
{
	hStatusTime->Text = msg;
}


// <-- clear file search list box. -->
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared data");
	hStatus->Text = "Cleared results";
	hFsListBox->Items->Clear();
}

// <-- retrive file names from message body with server details -->
array<String^>^ WPFCppCliDemo::getFileNameWithDetails(Message& msg)
{
	String^ sMsg = toSystemString(msg.body);
	std::vector<std::string> fileList = msg.split(toStdString(sMsg), FILES_SEP);
	array<String^>^ args = gcnew array<String^>(fileList.size());
	for(size_t i = 0; i < fileList.size(); i++)
	{
		if(msg.senderPort == stoi(toStdString(server1Port)))
		{
			args[i] = server1details + " "+ toSystemString(fileList[i]);
		}
		else if(msg.senderPort == stoi(toStdString(server2Port)))
		{
			args[i] = server2details + " " + toSystemString(fileList[i]);
		}
	}
	return args;
}

// <-- getMessage from receving queue and take actions after anaylze command type. -->
void WPFCppCliDemo::getMessage()
{
	while(true)
	{
		Message msg = pChann_->getMessage();
		String^ sMsg = toSystemString(msg.createHeaderMessage(msg));
		if(msg.commandType == GETFILES_RESPONSE || msg.commandType == TEXTSEARCH_RESPONSE)
		{
			if(msg.body.size() > 0)
			{
				array<String^>^ args = getFileNameWithDetails(msg);
				array<Object^>^ parms = gcnew array<Object^> { args };
				Action<array<String^>^>^ act = gcnew Action<array<String^>^>(this, &WPFCppCliDemo::updateListBox);
				Dispatcher->Invoke(act, parms); 
				
				end = std::chrono::steady_clock::now();
				typedef std::chrono::duration<int, std::milli> millisecs_t; millisecs_t duration(std::chrono::duration_cast<millisecs_t>(end - start));
				//std::cout << "\n\nTime Taken:" << duration.count() << "milliseconds.\n";
				String^ temp = toSystemString(std::to_string(duration.count()));
				sMsg = " Time taken : " + temp + " miliseconds.";
				Console::Write("\n ",sMsg);
				Action<String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusBox);
				Dispatcher->Invoke(act1, sMsg);
			}
			else
			{
				sMsg = "Files not found.";
				Console::Write("\n ", sMsg);
				Action<String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusBox);
				Dispatcher->Invoke(act1, sMsg);
			}
		}
		else if(msg.commandType == UPLOAD_ACK)
		{
			sMsg = "File Uploaded Successfully.";
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusBox);
			Dispatcher->Invoke(act, sMsg);  
		}
		else if(msg.commandType == DOWNLOAD_ACK)
		{
			sMsg = "File Downloaded Successfully.";
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusBox);
			Dispatcher->Invoke(act, sMsg);  
		}
	}
}

// <-- Parse Command Line Arguments -->
bool WPFCppCliDemo::ParseCommandLineArgs(array<System::String^>^ args)
{
	if(args->Length == 3)
	{
		clientdetails = args[0];
		server1details = args[1];
		server2details = args[2];

		array<System::String^>^ server1 = server1details->Split(':');
		array<System::String^>^ server2 = server2details->Split(':');
		array<System::String^>^ client = clientdetails->Split(':');
		if(server1->Length == 2)
		{
			server1Address = server1[0];
			server1Port = server1[1];
		}
		if(server2->Length == 2)
		{
			server2Address = server2[0];
			server2Port = server2[1];
		}
		if(client->Length == 2)
		{
			clientAddress = client[0];
			clientPort = client[1];
		}
		return true;
	}
	else
	{
		Console::WriteLine(L"\n Please give proper command line arguments. For more details check README file.s\n");
		return false;
	}
}

// <--  OnLoaded applicaton . -->
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
// <--  OnUnLoaded applicaton . -->
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");
	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo(args));
}
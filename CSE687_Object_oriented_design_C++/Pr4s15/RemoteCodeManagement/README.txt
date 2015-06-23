Author - Saurabh Patel
Course - CSE687 Project #4

Steps to start the project
-------------------------------------------------------------------------------------------------------------------
STEP1 - compile.bat 	// used to compile
STEP2 - run.bat	               // will run the executive with command line arguments.
	  
Notes :
-------------------------------------------------------------------------------
1)9085 is port on which server 1 will run, you can change from command line arguments.
2)9086 is port on which server 2 will run, you can change from command line arguments.
3)9080 is port on which Client run, that means WPF application, you can change that too from WPF application command line arguments.
4)client and server port can be change from command line arguments as describe below. This thing has been confirmed from Professor.
5) For File upload you can upload files from "upload_files" folder, which have sample .java .cpp and .cs files
6) Both server have two root folder which are "server1root" and "server2root".
7) File download will be save on "clientroot" folder.

Flow of application.
-------------------------------------------------------------------------------
There are 2 tabs in the GUI:

1)File Upload
	- User can upload the file to a selected server based on the selection of the check box.If no check box is selected user gets message on status bar regarding same.
	- User needs to select catagory from drop down list, If user wants to upload any file in root path of server  then user needs to select "root" catagory from that list. Otherwise user can go with different other catagories like JAVA,C++,DB,Thread etc.
	- After server and catagory selection user needs to select file using browse option and after browse user
	  needs to click on File upload button.
	- The files for Server1 and Server2 will be saved inside ./server1root/, server2root/ respectively.
2)File Search Text Search and Download
File Search
	- File Search can be done on second tab. 
	- Files can be searched from 2 servers based on check-box selection as I described above.
	- If User wish to search particular patterns or wildcards for files then user needs to enter patterns with    space separated in textbox of file pattern
	- For File search user needs to click on File Search button and in this case application will ignore text search texbox input. Actually text search textbox will be use in text search.
	- List of matched files are shown in the list box below those buttons. Results can be cleared if user press clear button.
Text Search
	- Text Search can be done on second tab.
	- User needs to follow same steps as described above, only user needs to enter regex pattern and text which user wants to search.
	- User can also provide patterns here too.
Download File
	- Once file search and text search list are in listbox, User can click on download buttong and download that particular selected files from server. Here User does not need to select any servers or pattern or catagory.
	- Application automatically take filename,filepath, server ip and port number from listbox selected value.


Regarding all request and response message, Status bar will be change where as it require

Please contact me on skpatel@syr.edu if you face any problems in compile or run.
Thanks
Saurabh
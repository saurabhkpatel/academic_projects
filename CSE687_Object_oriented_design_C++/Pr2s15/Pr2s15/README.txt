ORDER OF EXECUTION
==================
BUILDING/GENERATING THE XML DOCUMENT MODEL
-------------------------------------------------------------------------------------------------------------------
STEP1 - compile.bat 	// used to compile
STEP2 - run.bat	// will run the executive with command line arguments.

Please check below notes also.

Notes :-
1)
-Here i used two command line arguments, first is for xml file name and second is for string file name.
-For example : 
                    Debug\Executive.exe LectureNote.xml string.txt
					
-LectureNote.xml : is simple xml file, which will be useful to demonstrate requirements based on xml file.
-string.txt : is simple text file, where you can change string input to demonstrate requirements based on string input.
-If user will not provide this then application by default take automatic "LectureNote.xml" and "string.txt" files as input.

2) All xml write operations would be done in only one newly created xml file named "output.xml",so at last you will find only one modified xml file. Although some requirements will create other xml files, So please consider those files to verify output.


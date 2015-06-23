@echo off
echo           FileCatalogue, v1.0 - File Analysis Program
echo.
echo ***********************************************************
echo.

@echo off
echo  Demonstrate requirement #3 and #4 ,Path : ".", options : nothing.
echo ------------------------------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe .
echo This test case shows files of current directory.
echo.
Debug\FileCatalogueExecutor.exe .

@echo off
echo  Demonstrate requirement #3 and #4 ,Path : ".", options : /s, patterns.
echo ---------------------------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe . /s *.h *.cpp
echo This test case shows files of specified patterns from current directory and sub-dir.
echo.
Debug\FileCatalogueExecutor.exe . /s *.h *.cpp

@echo off
echo  Demonstrate requirement #3 and #4 , Path : ".", with patterns.
echo ---------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe . *.h *.cpp
echo This test case shows files of specified patterns from current directory only not sub-dir.
echo.
Debug\FileCatalogueExecutor.exe . *.h *.cpp

@echo off
echo Demonstrate requirement #5, Path : ".", options : /s and /d 
echo ------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe . /s /d
echo This test case shows files from current directory and sub-dir, moreover it shows duplicate files also.
echo.
Debug\FileCatalogueExecutor.exe . /s /d

@echo off
echo Demonstrate requirement #5, Path : ".", options : /s and /d 
echo ------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe . /d
echo This test case shows files from current directory and sub-dir, moreover it shows duplicate files also.
echo But there are no /s option so it will not show duplicate files.
echo.
Debug\FileCatalogueExecutor.exe . /d

@echo off
echo Demonstrate requirement #6,Path : ".", options : /s , with no patterns, /f"Hello world".
echo --------------------------------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe . /s /d /f"Hello world"
echo This test case shows files from current directory and sub-dir.
echo also will search "Hello world" text from the built in File Catalogue.
echo.
Debug\FileCatalogueExecutor.exe . /s /d /f"Hello world"

@echo off
echo Demonstrate requirement #7,No command line options.
echo --------------------------------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe
echo This test case shows files from current dir because program will take default file path and patterns.
echo.
Debug\FileCatalogueExecutor.exe

@echo off
echo Demonstrate requirement #8,Path : ".", options : /s , with no patterns.
echo --------------------------------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe . /s
echo This test case shows files from current dir and sub-dir
echo Moreover, it will ask user to enter text to search from file catalogue files.
echo.
Debug\FileCatalogueExecutor.exe . /s

@echo off
echo Demonstrate requirement #8,Path : ".", options : /s /d /fHello
echo --------------------------------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe ../ /s /d /fHello
echo This test case shows files from current dir and sub-dir, also list out of files from file catalogue which contains "Hello"
echo Moreover, it will ask user to enter text to search from file catalogue files.
echo.
Debug\FileCatalogueExecutor.exe ../ /s /d /fHello

@echo off
echo Demonstrate requirement #8,Path : ".", options : /s , with "*.cpp" patterns, Now enter search text "hello" with "*.h" and "*.cpp pattern"
echo ----------------------------------------------------------------------------------------------------------------------------------
echo Debug\FileCatalogueExecutor.exe /s
echo This test case shows files of specified patterns (*.cpp) from current dir and sub-directories.
echo Now, user have to enter "hello" text with "*.h" and "*.cpp" pattern like this : Hello *.h *.cpp
echo Above input will show files which have "hello" text. Here it will not search for *.h files because
echo file catalogue will not contains any *.h files as it built up with *.cpp pattern only.
echo.
Debug\FileCatalogueExecutor.exe /s
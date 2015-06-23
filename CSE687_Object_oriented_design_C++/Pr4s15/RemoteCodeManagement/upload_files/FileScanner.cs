///////////////////////////////////////////////////////////////////////////
// FileScanner.cs - It scans directory structure for given file pattern  //
//                                                                       //
// Language:    C#, .Net                                                 //
// Platform:    Lenovo G50 Windows 8                                     //
// Application: Project #2, Fall 2014                                    //
// Author:      Kandarpkumar Makwana, Syracuse University                //
//              (315) 751-5836, kjmakwan@syr.edu                         //
/////////////////////////////////////////////////////////////////////////// 
/*
 * Test stub Command Line Pattern: "directoryPath" "filePattern" <optional>"/S"
 * example: FileScanner.exe . *.cs /s
 * 
 * operations:
 * It scans the given directory for specified file pattern and returns list of files found there.
 * It uses delegates.
 * 
 * 
 * GetFilePaths(): It provides list of full path of all found files.
 * 
 * Version History:
 * ----------------
 * ver 1.0: september 2014
 * first release
 *
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Analyzer
{
    public class FileScanner
    {
        string path;
        List<string> patterns;
        Boolean isSubDir;
        List<string> filePaths=new List<string>();

        public FileScanner(string path, List<string> patterns, Boolean isSubDir)
        {
            this.path = path;
            this.patterns = patterns;
            this.isSubDir = isSubDir;
        }

        public void Register(Navigate nav)
        {
            nav.newDir += new Navigate.newDirHandler(OnDir);//--<registers to delegate>--
            nav.newFile += new Navigate.newFileHandler(OnFile);
        }

        public void OnDir(string dir)
        { }
        public void OnFile(string file)
        {
            string name = Path.GetFullPath(file);//--<action at file occurance.>--
            filePaths.Add(name);//----<adding file path in list>-----
                    }
        public List<string> GetFilePaths()
        {
            try
            {
                Navigate nav = new Navigate(isSubDir);
                Register(nav);
                foreach (string pattern in patterns)
                    nav.go(path, pattern);
                return filePaths;
            }
            catch(Exception e)
            {
                Console.WriteLine("Bad path. Correct it."+e.Message);
                List<string> nil = new List<string>();
                nil.Add("null");
                return nil;
            }
        }
#if(TEST_FILE_SCANNER)
        [STAThread]
        static void Main(string[] args)
        {
            string path;
            bool optS = false;
            List<string> patterns = new List<string>();

            if (args.Length > 0)
            {
                path = args[0];
                if (args[2].ToUpper().Equals("/S"))
                    optS = true;
                patterns.Add(args[1]);
            }
            else
                path = Directory.GetCurrentDirectory();
            FileScanner fs = new FileScanner(path, patterns, optS);//--<testing FileScanner>---
            List<string> files= fs.GetFilePaths();
            
            foreach(string file in files)
            {
                Console.WriteLine(file);
            }
        }
#endif
    }
}

///////////////////////////////////////////////////////////////////////////
// Analyzer.cs -The core part for analysis. It detects types and store
// Language:    C#, .Net Framework                                       //
// Platform:    Lenovo G50 Windows 8                                     //
// Application: Project #2, Fall 2014                                    //
// Author:      Kandarpkumar Makwana, Syracuse University                //
//              (315) 751-5836, kjmakwan@syr.edu                         //
/////////////////////////////////////////////////////////////////////////// 
/*
 * TestStub:
 * command line argument pattern : directory-path
 * example: "../../"
 * 
 * Operations:
 * Core analysis .
 * It has two phases separated as 2 methods in here.
 * FirstPhase uses regular parser provided by Dr Fawcett to fetch all namespaces, types, methods from 
 * the file set and measure function complexity. It stores all this information in TypeFunctionRepo
 * SecondPhase builds another parser according to requirements using new rules and actions. It updates 
 * relation Information in typeFunctionRepo. 
 * 
 * First Phase:
 * Using parser provided by Dr Fawcett fetches locations table with entering scope line and leaving scope.
 * Scans the locations table and looks for namespaces, type, functions and control scopes and at occurance of any of them 
 * stores in typefunctionrepo. Increases the complexity count for function at occurance of control structures.
 * 
 * Second Phase:
 * Using newly created rules and actions new parser is built for 2nd phase. all files are scanned using this function.
 * Maintenance History:
 * -------------------
 * ver 1.3:
 *  added enum and delegate type detection for analysis
 * ver 1.2:
 *  added second phase
 * ver 1.0:
 *  first release. added first phase analysis
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using CodeAnalysis;

namespace Analyzer
{
    public class AnalyzerClass
    {
        bool optR=false;
        List<string> files;
        TypeFunctionRepo typefunctionrepo;
        public AnalyzerClass(List<string> files, bool optR)
        {
            this.files=files;
            this.optR=optR;
            typefunctionrepo = new TypeFunctionRepo();
        }
        public int GetNameSpaceIndex(string namespacename)//------<If namespace is previously stored in typefunctionrepo, detects namespaceindex in repository >------
        {
            int indexNamespace = 0;
            foreach (NamespaceElem nse in typefunctionrepo.namespaces)
            {
                if (nse.namesp == namespacename)
                    break;
                else
                    indexNamespace++;
            }
            if (indexNamespace == typefunctionrepo.namespaces.Count)
            {
                NamespaceElem namespaceelem = new NamespaceElem(namespacename);
                typefunctionrepo.namespaces.Add(namespaceelem);
            }
            return indexNamespace;
        }
        public void FirstPhase()
        {
            foreach (string file in files)
            {
                CSsemi.CSemiExp semi = new CSsemi.CSemiExp();
                semi.displayNewLines = false;
                if (!semi.open(file))
                {
                    Console.Write("\n Can't open {0}\n\n", file);
                }
                BuildCodeAnalyzer builder = new BuildCodeAnalyzer(semi);
                Parser parser = builder.build();//------<regular parser for first phase analysis>------
                try
                {
                    while (semi.getSemi())
                        parser.parse(semi);
                }
                catch (Exception ex)
                { Console.Write("\n\n  {0}\n", ex.Message);}
                Repository rep = Repository.getInstance();
                List<Elem> locations = rep.locations;//------<gets location table for each file>------
                TypeElem typeelem;FuncElem funcelem;
                int indexNamespace = 0,indexBigTypes = 0;
                bool functionScope = false;
                string namespacename;
                if (locations.Count == 0) continue;
                if (locations[0].type != "namespace")
                {
                    namespacename = "global_namespace";
                    indexNamespace = GetNameSpaceIndex(namespacename);//------<index of namespace, if it is already stored in the typefunctionrepo.>------
                }
                else
                {
                    namespacename = locations[0].name;
                }
                foreach (Elem e in locations)
                {
                    if (e.type == "namespace")//-------<adds namespace elem in typefunctionrepo>----------
                    {
                        namespacename = e.name;
                        indexNamespace = GetNameSpaceIndex(namespacename);
                        functionScope = false;
                    }
                    if (e.type == "class" || e.type == "struct" || e.type == "interface")//-------------<types: class, struct and interface is added  in typefuncrepo>----------
                    {
                        functionScope = false;
                        typeelem = new TypeElem(e.type, e.name);
                        typefunctionrepo.namespaces[indexNamespace].types.Add(typeelem);
                    }
                    else if (e.type == "function")//-----<checks for funcion occurance in locations table>--------
                    {
                        funcelem = new FuncElem(e.name, e.end - e.begin);
                        List<TypeElem> types = typefunctionrepo.namespaces[indexNamespace].types;
                        try
                        {   int i;
                            for (i = types.Count - 1; i >= 0 && (types[i].type == "enum" || types[i].type == "delegate" || types[i].type == "struct"); --i) ;

                            indexBigTypes = i;
                            types[indexBigTypes].functions.Add(funcelem);
                            functionScope = true;//-------<turns on functionscope to enable complexity count at control or braceless scope encounter. >----------
                        }
                        catch (Exception tperror) { }}
                    else if ((e.type == "control" || e.type == "bracecontrol") && !optR && functionScope)//-------<checks for annonymous control and braceless scope and >----------
                    {
                        List<FuncElem> functions = typefunctionrepo.namespaces[indexNamespace].types[indexBigTypes].functions;//-------<inceases complexity for functions >----------
                        functions[functions.Count - 1].complexity++;}
                    else if (e.type == "enum" || e.type == "delegate")//----<detects for enum and delegates types>-----
                    {   typeelem = new TypeElem(e.type, e.name);
                        typefunctionrepo.namespaces[indexNamespace].types.Add(typeelem);
                    }
                }
            }
        }    
            
        public void secondPhase(bool optR)
        {
            if (!optR)//----<if /R is not specified 2nd phase is not executed>----
                return;
            BuildSecondPhaseAnalyzer builder = new BuildSecondPhaseAnalyzer();//----<using newly created rules and actions parser for 2nd phase is built. >------
            Parser parser = builder.build();
            foreach (string file in files)
            {
                CSsemi.CSemiExp semi = new CSsemi.CSemiExp();
                semi.displayNewLines = false;
                if (!semi.open(file as string))
                {
                    Console.Write("\n second phase Can't open {0}\n\n", file);
                }
                try
                {
                    while (semi.getSemi())
                        parser.parse(semi);
                }
                catch (Exception ex) {  }
                }
            }
#if(TEST_ANALYZER)
        static void Main(string[] args)
        {
            string dir;
            bool optR=false;
            if(args.Length>1)
            {
                dir = args[0];
            }
            else { dir = "../../";  }
            List<string> pat = new List<string>();
            pat.Add("*.cs");
            FileScanner fs = new FileScanner(dir, pat, true);
            List<string> files = fs.GetFilePaths();
            AnalyzerClass analyzer = new AnalyzerClass(files, optR);//--<testing analyzerclass>--
            analyzer.FirstPhase();
            analyzer.secondPhase(optR);

            TypeFunctionRepo tfrepo = TypeFunctionRepo.getInstance();
            
            if (tfrepo.namespaces.Count != 0)
            {
                StringBuilder temp = new StringBuilder();
                foreach (NamespaceElem nse in tfrepo.namespaces)
                {
                    temp.Append("Namespace: " + nse.namesp).Append("\n");
                    
                    foreach (TypeElem te in nse.types)
                    {
                        temp.Append("   " + te.type + ": " + te.name).Append("\n");
                        if (optR)
                            foreach (Relation r in te.relations)
                            {
                                temp.Append("       " + r.relation + ": " + r.type).Append("\n");
                            }
                        foreach (FuncElem fe in te.functions)
                        {
                            //noFunc++;
                            temp.Append("          function: " + fe.fname + ": size: " + fe.size);
                            if (!optR)
                            {
                                temp.Append(": complexity: " + fe.complexity);
                            }
                            temp.Append("\n");
                        }temp.Append("\n ");
                    }temp.Append("\n \n");
                    Console.Write(temp.ToString());
                }
            }
        }
         
#endif
    }
}

///////////////////////////////////////////////////////////////////////
// RulesAndActions.cs - Parser rules specific to an application      //
// ver 2.1                                                           //
// Language:    C#, 2008, .Net Framework 4.0                         //
// Platform:    Dell Precision T7400, Win7, SP1                      //
// Application: Demonstration for CSE681, Project #2, Fall 2011      //
// Author:      Jim Fawcett, CST 4-187, Syracuse University          //
//              (315) 443-3948, jfawcett@twcny.rr.com                //
// Updated by:  Kandarp Makwana, Syracuse University                 //
//              (315) 751-5836, kjmakwan@syr.edu                     //
///////////////////////////////////////////////////////////////////////
/*
 *Added :
 *elements:
 *
 *TypeFunctionRepo
 *TypeElem
 *NamespaceElem
 *FuncElem
 *relation
 *
 * rules:
 *DetectDelegate
 *DetectEnum
 *DetectClassModified for 2nd phase 
 *SetIndexNamespace
 *SetIndexType
 *
 * Actions:
 *AddAggregation
 *AddComposition
 *AddInheritence
 *AddUsingRel
 *
 * Builder:
 * BuildSecondPhaseAnalyzer
 * 
 * As mentioned above a lot of modifiation and addition are made to construct analyzer.
 * 
 * Package Operations:
 * -------------------
 * RulesAndActions package contains all of the Application specific
 * code required for most analysis tools.
 *
 * It defines the following Four rules which each have a
 * grammar construct detector and also a collection of IActions:
 *   - DetectNameSpace rule
 *   - DetectClass rule
 *   - DetectFunction rule
 *   - DetectScopeChange
 *   
 *   Three actions - some are specific to a parent rule:
 *   - Print
 *   - PrintFunction
 *   - PrintScope
 * 
 * The package also defines a Repository class for passing data between
 * actions and uses the services of a ScopeStack, defined in a package
 * of that name.
 *
 * Note:
 * This package does not have a test stub since it cannot execute
 * without requests from Parser.
 *  
 */
/* Required Files:
 *   IRuleAndAction.cs, RulesAndActions.cs, Parser.cs, ScopeStack.cs,
 *   Semi.cs, Toker.cs
 *   
 * Build command:
 *   csc /D:TEST_PARSER Parser.cs IRuleAndAction.cs RulesAndActions.cs \
 *                      ScopeStack.cs Semi.cs Toker.cs
 *   
 * Maintenance History:
 * --------------------
 * ver 2.5 :Sept 2014 by Kandarpkumar Makwana
 *   All modifications are discussed above.
 * ver 2.2 : 24 Sep 2011
 * - modified Semi package to extract compile directives (statements with #)
 *   as semiExpressions
 * - strengthened and simplified DetectFunction
 * - the previous changes fixed a bug, reported by Yu-Chi Jen, resulting in
 * - failure to properly handle a couple of special cases in DetectFunction
 * - fixed bug in PopStack, reported by Weimin Huang, that resulted in
 *   overloaded functions all being reported as ending on the same line
 * - fixed bug in isSpecialToken, in the DetectFunction class, found and
 *   solved by Zuowei Yuan, by adding "using" to the special tokens list.
 * - There is a remaining bug in Toker caused by using the @ just before
 *   quotes to allow using \ as characters so they are not interpreted as
 *   escape sequences.  You will have to avoid using this construct, e.g.,
 *   use "\\xyz" instead of @"\xyz".  Too many changes and subsequent testing
 *   are required to fix this immediately.
 * ver 2.1 : 13 Sep 2011
 * - made BuildCodeAnalyzer a public class
 * ver 2.0 : 05 Sep 2011
 * - removed old stack and added scope stack
 * - added Repository class that allows actions to save and 
 *   retrieve application specific data
 * - added rules and actions specific to Project #2, Fall 2010
 * ver 1.1 : 05 Sep 11
 * - added Repository and references to ScopeStack
 * - revised actions
 * - thought about added folding rules
 * ver 1.0 : 28 Aug 2011
 * - first release
 *
 * Planned Modifications (not needed for Project #2):
 * --------------------------------------------------
 * - add folding rules:
 *   - CSemiExp returns for(int i=0; i<len; ++i) { as three semi-expressions, e.g.:
 *       for(int i=0;
 *       i<len;
 *       ++i) {
 *     The first folding rule folds these three semi-expression into one,
 *     passed to parser. 
 *   - CToker returns operator[]( as four distinct tokens, e.g.: operator, [, ], (.
 *     The second folding rule coalesces the first three into one token so we get:
 *     operator[], ( 
 */
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace CodeAnalysis
{
    public class FuncElem//------<to store data of functions>------
    {
        public string fname { get; set; }
        public int size = 0;
        public int complexity = 1;
        public bool exists = false;




        public FuncElem(string name, int size)
        {
            fname = name;
            this.size = size;
        }
        public override string ToString()
        {
            StringBuilder temp = new StringBuilder();
            temp.Append("{");
            temp.Append(String.Format("{0,-10}", fname)).Append(" : ");
            temp.Append(String.Format("{0,-10}", size)).Append(" : ");
            temp.Append(String.Format("{0,-10}", complexity)).Append("\n");
            return temp.ToString();
        }
    }
    public class TypeElem//------<to store data of types>------
    {

        public string type { get; set; }
        public string name { get; set; }
        public List<FuncElem> functions = new List<FuncElem>();
        public List<Relation> relations = new List<Relation>();
        public bool exists = false;

        public TypeElem(string type, string name)
        {
            this.type = type;
            this.name = name;
        }

        public void AddRel(Relation rel)//------<to add corresponding relationship with other elements>------
        {
            TypeFunctionRepo tfr=TypeFunctionRepo.getInstance();
            bool flag=false;
            foreach(Relation r in relations)
            {
                if (r.type.Equals(rel.type) && r.relation.Equals(rel.relation))
                    flag = true;
            }
            if (!flag)
                tfr.namespaces[tfr.indexNameSpace].types[tfr.indexType].relations.Add(rel);


        }
        


        public override string ToString()
        {
            StringBuilder temp = new StringBuilder();
            temp.Append("{");
            //temp.Append(String.Format("{0,-10}", namesp)).Append(" : ");
            temp.Append(String.Format("{0,-10}", type)).Append(" : ");
            temp.Append(String.Format("{0,-10}", name)).Append("\n");
            foreach (FuncElem f in functions)
            {
                temp.Append(f.ToString());
            }
            //temp.Append("\n");

            foreach (Relation r in relations)
            {
                temp.Append(String.Format("{0,-10}", name));
                temp.Append(String.Format("{0,-10}", r.relation)).Append(" : ");
                temp.Append(String.Format("{0,-10}", r.type)).Append("\n");
            }

            return temp.ToString();
        }
    }
    public class NamespaceElem//------<To store data about Namespaces>------
    {
        public string namesp { get; set; }
        public List<TypeElem> types = new List<TypeElem>();
        public NamespaceElem(string namesp)
        {
            this.namesp = namesp;
        }
        public override string ToString()
        {
            StringBuilder temp = new StringBuilder();
            temp.Append("{");
            temp.Append(String.Format("{0,-10}", namesp)).Append(" : \n");
            foreach (TypeElem te in types)
            {
                temp.Append(te.ToString());
            }
            return temp.ToString();
        }

    }
    public class Elem  // holds scope information
    {
        public string type { get; set; }
        public string name { get; set; }
        public int begin { get; set; }
        public int end { get; set; }
        public bool exists = true;

        public override string ToString()
        {
            StringBuilder temp = new StringBuilder();
            temp.Append("{");
            temp.Append(String.Format("{0,-10}", type)).Append(" : ");
            temp.Append(String.Format("{0,-10}", name)).Append(" : ");
            temp.Append(String.Format("{0,-5}", begin.ToString()));  // line of scope start
            temp.Append(String.Format("{0,-5}", end.ToString()));    // line of scope end
            temp.Append("}");
            return temp.ToString();
        }
    }
    public class Relation//------<describes the relation and name of type>------
    {
        public string type
        {
            get;
            set;
        }
        public string relation
        {
            get;
            set;
        }
    }
    public class TypeFunctionRepo//------<All the data after first and second analysis is stored in it>------
    {
        static TypeFunctionRepo instance;
        List<NamespaceElem> namespaces_ = new List<NamespaceElem>();
        public int indexNameSpace;
        public int indexType;
        public int indexClass;
        public TypeFunctionRepo()
        {
            instance = this;
        }
        public static TypeFunctionRepo getInstance()
        {
            return instance;
        }
        public List<NamespaceElem> namespaces
        {
            get { return namespaces_; }
        }
        
    }

    public class Repository
    {
        ScopeStack<Elem> stack_ = new ScopeStack<Elem>();
        List<Elem> locations_ = new List<Elem>();

        static Repository instance;


        public Repository()
        {
            instance = this;
        }

        public static Repository getInstance()
        {
            return instance;
        }
        // provides all actions access to current semiExp

        public CSsemi.CSemiExp semi
        {
            get;
            set;
        }

        // semi gets line count from toker who counts lines
        // while reading from its source

        public int lineCount  // saved by newline rule's action
        {
            get { return semi.lineCount; }
        }
        public int prevLineCount  // not used in this demo
        {
            get;
            set;
        }
        // enables recursively tracking entry and exit from scopes

        public ScopeStack<Elem> stack  // pushed and popped by scope rule's action
        {
            get { return stack_; }
        }
        // the locations table is the result returned by parser's actions
        // in this demo

        public List<Elem> locations
        {
            get { return locations_; }
        }

    }
    /////////////////////////////////////////////////////////
    // pushes scope info on stack when entering new scope

    public class PushStack : AAction
    {
        Repository repo_;

        public PushStack(Repository repo)
        {
            repo_ = repo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            Elem elem = new Elem();
            elem.type = semi[0];  // expects type
            elem.name = semi[1];  // expects name
            elem.begin = repo_.semi.lineCount - 1;
            elem.end = 0;
            repo_.stack.push(elem);
            
            repo_.locations.Add(elem);

            if (AAction.displaySemi)
            {
                Console.Write("\n  line# {0,-5}", repo_.semi.lineCount - 1);
                Console.Write("entering ");
                string indent = new string(' ', 2 * repo_.stack.count);
                Console.Write("{0}", indent);
                this.display(semi); // defined in abstract action
            }
            if (AAction.displayStack)
                repo_.stack.display();
        }
    }
    public class SetIndexNameSpace : AAction//------<at every occurance of namespace it updates indexNamespace in TypeFuncrepo>------
    {
        TypeFunctionRepo tfrepo;
        public SetIndexNameSpace(TypeFunctionRepo tfrepo)
        {
            this.tfrepo = tfrepo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            int indexNameSpace = 0;
            foreach (NamespaceElem nse in tfrepo.namespaces)
            {
                //Console.WriteLine("nse.name" + nse.namesp + "semi[1]" + semi[1]);
                if (nse.namesp == semi[1])
                    break;
                ++indexNameSpace;
            }
            tfrepo.indexNameSpace = indexNameSpace;
            tfrepo.indexType = -1;
        }
    }

    public class SetIndexType : AAction//------<At every occurance of type it sets indexType in typefunctionrepo>------
    {
        TypeFunctionRepo tfrepo;
        public SetIndexType(TypeFunctionRepo tfrepo)
        {
            this.tfrepo = tfrepo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            try
            {
                int indexClass = 0;
                bool found = false;
                foreach (TypeElem te in tfrepo.namespaces[tfrepo.indexNameSpace].types)
                {
                    //Console.WriteLine("te.name"+te.name+"semi[1]"+semi[1]);
                    if (te.name == semi[1])
                    {
                        found = true;
                        break;
                    }
                    ++indexClass;
                }
                if (found)
                    tfrepo.indexType = indexClass;
                else
                {
                    tfrepo.indexNameSpace = GetNameSpaceIndex("global_namespace");//------<If the file doesn't specify namespace, the elements defined in that file is stored in global_namespace>----------
                    tfrepo.indexType = -1;
                    doAction(semi);
                }
                if (semi[0].Equals("class"))
                    tfrepo.indexClass = tfrepo.indexType;
            }
            catch (Exception e)
            {
                Console.WriteLine("repo indexNamespace " + tfrepo.indexNameSpace + " repo indexType " + tfrepo.indexType + " count of namespaces " + tfrepo.namespaces.Count + " Count of types in that namespace"); //+ tfrepo.namespaces[tfrepo.indexNameSpace].types.Count);
                Console.WriteLine(e.Message);
                Console.WriteLine("SetIndexType is creating trouble");

            }
        }
    }
    public class AddInheritence : AAction//------<Adds inheritance to tyefunctionrepo relations>------
    {
        TypeFunctionRepo tfrepo;
        public AddInheritence(TypeFunctionRepo tfrepo)
        {
            this.tfrepo = tfrepo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            try
            {
                int index = semi.FindFirst(":");
                //Console.Write("in addinheritence" + index);
                string type;
                if (index != -1)
                {


                    Relation relation;
                    if (!(type = TypeDefined(semi[index + 1])).Equals("null"))
                    {
                        relation = new Relation();
                        relation.relation = "inheritance";
                        relation.type = type;
                        tfrepo.namespaces[tfrepo.indexNameSpace].types[tfrepo.indexType].AddRel(relation);
                    }
                    int end = semi.count - 1;
                    while ((index = semi.semiExp.LastIndexOf("(", end)) != -1)
                    {
                        if (!(type = TypeDefined(semi[index + 1])).Equals("null"))
                        {
                            relation = new Relation();
                            relation.relation = "inheritance";
                            relation.type = type;


                            tfrepo.namespaces[tfrepo.indexNameSpace].types[tfrepo.indexType].AddRel(relation);
                        }
                        end = index - 1;
                    }


                }
            }
            catch (Exception e)
            {
                
            }
        }
    }
    public class AddAggregation : AAction
    {
        TypeFunctionRepo tfrepo;
        public AddAggregation(TypeFunctionRepo tfrepo)
        {
            this.tfrepo = tfrepo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            int index = semi.FindFirst("new");
            string type;
            if (!(type = TypeDefined(semi[index + 1])).Equals("null"))
            {
                Relation relation = new Relation();
                relation.relation = "aggregation";
                relation.type = type;

                tfrepo.namespaces[tfrepo.indexNameSpace].types[tfrepo.indexType].AddRel(relation);

            }
          }
    }
    public class AddUsingRel : AAction
    {
        TypeFunctionRepo tfrepo;
        public AddUsingRel(TypeFunctionRepo tfrepo)
        {
            this.tfrepo = tfrepo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            int end = semi.FindFirst(")");
            int start = semi.FindFirst("(");
            string type;
            try
            {
                for (end -= 2; end > start; end -= 2)
                {
                    if (!(type = TypeDefined(semi[end])).Equals("null"))
                    {

                        Relation relation = new Relation();
                        relation.relation = "using";
                        relation.type = type;

                        tfrepo.namespaces[tfrepo.indexNameSpace].types[tfrepo.indexType].AddRel(relation);
                    }

                }
            }
            catch (Exception e) { }
        }
    }

    public class AddComposition : AAction
    {
        TypeFunctionRepo tfrepo;
        public AddComposition(TypeFunctionRepo tfrepo)
        {
            this.tfrepo = tfrepo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            string type;
                if (tfrepo.indexType != -1)
                {   Relation relation = new Relation();
                    relation.relation = "composition";
                    if (semi.Contains(";") != -1)
                    {
                        if (!(type = TypeDefined(semi[1])).Equals("null"))
                        {
                            relation.type = type;
                            tfrepo.namespaces[tfrepo.indexNameSpace].types[tfrepo.indexType].AddRel(relation);
                            return;
                        }
                        if (semi.Contains(".") != -1)
                        {
                            if (!(type = TypeDefined(semi[semi.FindFirst(".") - 1])).Equals("null"))
                            {
                                relation.type = type;
                                tfrepo.namespaces[tfrepo.indexNameSpace].types[tfrepo.indexType].AddRel(relation);
                                return;
                            }
                        }
                    }
                    else if (semi.Contains("{") != -1 && semi.Contains("struct") != -1)
                    {
                            relation.type = TypeDefined(semi[semi.FindFirst("struct") + 1]);
                            tfrepo.namespaces[tfrepo.indexNameSpace].types[tfrepo.indexType].AddRel(relation);
                            return;
                    }
                    if (semi.Contains("enum") != -1)
                    {   relation.type = TypeDefined(semi[semi.FindFirst("enum") + 1]);//semi[index+1];
                        tfrepo.namespaces[tfrepo.indexNameSpace].types[tfrepo.indexType].AddRel(relation);
                        return;
                    }

                }
        }
    }
    /////////////////////////////////////////////////////////
    // pops scope info from stack when leaving scope

    public class PopStack : AAction
    {
        Repository repo_;

        public PopStack(Repository repo)
        {
            repo_ = repo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            Elem elem;
            try
            {
                do
                {
                    elem = repo_.stack.pop();
                } while (elem.type == "bracecontrol");
                for (int i = repo_.locations.Count - 1; i >= 0; --i)
                {
                    Elem temp = repo_.locations[i];
                    if (temp.type == "bracecontrol") continue;
                    if (elem.type == temp.type && elem.name == temp.name && (repo_.locations[i]).end == 0)
                    {
                                (repo_.locations[i]).end = repo_.semi.lineCount;
                                break;
                    }
                }
            }
            catch
            {return;}
            CSsemi.CSemiExp local = new CSsemi.CSemiExp();
            local.Add(elem.type).Add(elem.name);
            if (local[0] == "control")
                return;
             if (AAction.displaySemi)
            {
                Console.Write("\n  line# {0,-5}", repo_.semi.lineCount);
                Console.Write("leaving  ");
                string indent = new string(' ', 2 * (repo_.stack.count + 1));
                Console.Write("{0}", indent);
                this.display(local); // defined in abstract action
            }
        }
    }
    ///////////////////////////////////////////////////////////
    // action to print function signatures - not used in demo

    public class PrintFunction : AAction
    {
        Repository repo_;

        public PrintFunction(Repository repo)
        {
            repo_ = repo;
        }
        public override void display(CSsemi.CSemiExp semi)
        {
            Console.Write("\n    line# {0}", repo_.semi.lineCount - 1);
            Console.Write("\n    ");
            for (int i = 0; i < semi.count; ++i)
                if (semi[i] != "\n" && !semi.isComment(semi[i]))
                    Console.Write("{0} ", semi[i]);
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            this.display(semi);
        }
    }
    /////////////////////////////////////////////////////////
    // concrete printing action, useful for debugging

    public class Print : AAction
    {
        Repository repo_;

        public Print(Repository repo)
        {
            repo_ = repo;
        }
        public override void doAction(CSsemi.CSemiExp semi)
        {
            Console.Write("\n  line# {0}", repo_.semi.lineCount - 1);
            this.display(semi);
        }
    }
    /////////////////////////////////////////////////////////
    // rule to detect namespace declarations

    public class DetectNamespace : ARule
    {
        public override bool test(CSsemi.CSemiExp semi)
        {
            int index = semi.Contains("namespace");
            if (index != -1)
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                // create local semiExp with tokens for type and name
                local.displayNewLines = false;
                local.Add(semi[index]).Add(semi[index + 1]);
                doActions(local);
                return true;
            }
            return false;
        }
    }
    /////////////////////////////////////////////////////////
    // rule to dectect class definitions

    public class DetectClass : ARule
    {
        public override bool test(CSsemi.CSemiExp semi)
        {
            int indexCL = semi.Contains("class");
            int indexIF = semi.Contains("interface");
            int indexST = semi.Contains("struct");

            int index = Math.Max(indexCL, indexIF);
            index = Math.Max(index, indexST);
            if (index != -1)
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.displayNewLines = false;
                local.Add(semi[index]).Add(semi[index + 1]);
                doActions(local);
                return true;
            }
            return false;
        }
    }
    public class DetectClassModified : ARule //------------<rule modified for second phase analysis>---------------
    {
        public override bool test(CSsemi.CSemiExp semi)
        {
            int indexCL = semi.Contains("class");
            int indexIF = semi.Contains("interface");
            int index = Math.Max(indexCL, indexIF);
            if (index != -1)
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.displayNewLines = false;
                for (; index < semi.count; ++index)
                    local.Add(semi[index]);
                try
                {
                    doActions(local);
                }
                catch (Exception e){}
                return true;
            }
            return false;
        }
    }
        public class DetectEnum : ARule//------<Enum detection rule>------
    {

        public override bool test(CSsemi.CSemiExp semi)
        {
            if (semi[semi.count - 1] != "{")
                return false;

            int index = semi.FindFirst("enum");
            if (index > 0)
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.Add("enum").Add(semi[index + 1]);
                doActions(local);
                return true;
            }
            return false;
        }
    }
    public class DetectDelegate : ARule
    {

        public override bool test(CSsemi.CSemiExp semi)
        {
            int index = semi.FindFirst("delegate");
            if (index > 0 && semi[index + 3] == "(")
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.Add("delegate").Add(semi[index + 2]);
                doActions(local);
                return true;
            }
            else if (index > 0 && semi[index + 1] == "(")
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.Add("delegate").Add(semi[index + 2]);
                doActions(local);
                return true;
            }
            index = semi.FindFirst("Action");
            if (index > 0 && semi[index + 1] == "<" && semi[index + 3] == ">")
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.Add("delegate").Add(semi[index + 4]);
                doActions(local);
                return true;
            }
            index = semi.FindFirst("Func");
            if (index > 0 && semi[index + 1] == "<" && semi[index + 7] == ">")
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.Add("delegate").Add(semi[index + 8]);
                doActions(local);
                return true;
            }
            return false;
        }
    }


    public class DetectFunctionModified : ARule//------<modified rule for 2nd phase analysis. modified to detect using rlationship>------
    {

        public override bool test(CSsemi.CSemiExp semi)
        {
            if (semi[semi.count - 1] != "{")
                return false;

            int index = semi.FindFirst("(");
            if (index > 0 && !isSpecialToken(semi[index - 1]))
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                for (; index < semi.count; ++index)
                    local.Add(semi[index]);
                doActions(local);
                return true;
            }
            return false;
        }
    }

    public class DetectFunction : ARule
    {

        public override bool test(CSsemi.CSemiExp semi)
        {
            if (semi[semi.count - 1] != "{")
                return false;

            int index = semi.FindFirst("(");
            if (index > 0 && !isSpecialToken(semi[index - 1]))
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.Add("function").Add(semi[index - 1]);
                doActions(local);
                return true;
            }
            return false;
        }
    }
    /////////////////////////////////////////////////////////
    // detect entering anonymous scope
    // - expects namespace, class, and function scopes
    //   already handled, so put this rule after those
    public class DetectAnonymousScope : ARule//------<modified rule and narrowed down to specific cases to count complexity accurately>------
    {
        public override bool test(CSsemi.CSemiExp semi)
        {
            int index = semi.Contains("{");
            if (index != -1)
            {   int ind = semi.FindFirst("(");
                try
                {   if (ind != -1)
                    {if (isSpecialToken(semi[ind - 1]))
                        {CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                            local.displayNewLines = false;
                            local.Add("control").Add("anonymous");
                            doActions(local);
                            return true;
                        }
                    }
                    if (semi.Contains("else") == 1 || semi.Contains("do") == 1 || semi.Contains("try") == 1 || semi.Contains(":") == index - 2 || index == 1)
                    {CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                        local.displayNewLines = false;
                        local.Add("control").Add("anonymous");
                        doActions(local);
                        return true;
                    } }
                catch (Exception e){ }} 
            index = semi.Contains(";");
            if (index != -1)
            {
                int end = semi.count - 1;
                int ind;
                bool return_ = false;
                int tp;
                if ((tp = semi.Contains("else")) == 1 && !semi[tp + 1].Equals("if")||semi[index-1].Equals("break"))//-------<detects braceless scope.>----------
                {
                    CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                    local.displayNewLines = false;
                    local.Add("bracecontrol").Add("braceless scope");
                    doActions(local);
                    return_ = true;
                }
                while ((ind = semi.semiExp.LastIndexOf("(", end)) != -1)
                {   if (isSpecialToken(semi[ind - 1]))
                    {   CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                        local.displayNewLines = false;
                        local.Add("bracecontrol").Add(semi[ind + 1]);
                        doActions(local);
                        return_ = true; }
                    end = ind - 1;}
                return return_;}
           return false;}
    }
    /////////////////////////////////////////////////////////
    // detect leaving scope

    public class DetectLeavingScope : ARule
    {
        public override bool test(CSsemi.CSemiExp semi)
        {
            int index = semi.Contains("}");
            if (index != -1)
            {
                doActions(semi);
                return true;
            }
            int ind = semi.Contains(";");
            if (ind != -1)
            {
                if (semi[0] == "if" || semi[0] == "else" || (semi[0] == "else" && semi[1] == "if") || semi[0] == "for" || semi[0] == "while" || semi[0] == "foreach")
                {
                    doActions(semi);
                    return true;
                }
            }
            return false;
        }
    }
    public class DetectAggregation : ARule
    {
        public override bool test(CSsemi.CSemiExp semi)
        {

            int index = semi.FindFirst("new");

            if (index != -1 && semi.Contains(";") != -1)
            {   doActions(semi);
                return true;
            }
            return false;
        }
    }
    public class DetectStructModified : ARule//------<detects struct modified for 2nd phase analysis>------
    {
        public override bool test(CSsemi.CSemiExp semi)
        {
            int index = semi.Contains("struct");
            if (index != -1)
            {
                CSsemi.CSemiExp local = new CSsemi.CSemiExp();
                local.displayNewLines = false;
                for (; index < semi.count; ++index)
                    local.Add(semi[index]);
                try
                {
                    doActions(local);
                }
                catch (Exception e){}
                return true;
            }
            return false;
        }
    }
    public class DetectComposition : ARule
    {
        public override bool test(CSsemi.CSemiExp semi)
        {
            if (semi.Contains(";") != -1)
            {
                if (semi.Contains(".") != -1 && semi.Contains("=") != -1)
                    {
                        doActions(semi);
                        return true;
                    }
                    else if (semi.Contains(".") == -1 && semi.Contains("=") == -1)
                    {
                        doActions(semi);
                        return true;
                    }
            }
            return false;

        }
    }
        public class BuildCodeAnalyzer
        {
            Repository repo = new Repository();
            public BuildCodeAnalyzer(CSsemi.CSemiExp semi)
            {
                repo.semi = semi;
            }
            public virtual Parser build()
            {
                Parser parser = new Parser();
                AAction.displaySemi = false;
                AAction.displayStack = false;  
                PushStack push = new PushStack(repo);
                // capture namespace info
                DetectNamespace detectNS = new DetectNamespace();
                detectNS.add(push);
                parser.add(detectNS);

                // capture class info
                DetectClass detectCl = new DetectClass();
                detectCl.add(push);
                parser.add(detectCl);

                // capture enum info----<added in update>-----
                DetectEnum detectE = new DetectEnum();
                detectE.add(push);
                parser.add(detectE);

                // capture delegate info--<added in update>----
                DetectDelegate detectD = new DetectDelegate();
                detectD.add(push);
                parser.add(detectD);

                // capture function info
                DetectFunction detectFN = new DetectFunction();
                detectFN.add(push);
                parser.add(detectFN);

                // handle entering anonymous scopes, e.g., if, while, etc.
                DetectAnonymousScope anon = new DetectAnonymousScope();
                anon.add(push);
                parser.add(anon);

                // handle leaving scopes
                DetectLeavingScope leave = new DetectLeavingScope();
                PopStack pop = new PopStack(repo);
                leave.add(pop);
                parser.add(leave);

                // parser configured
                return parser;
            }
        }
        public class BuildSecondPhaseAnalyzer //------<new rules for second phase analysis is added here>------
        {
            TypeFunctionRepo typefunctionrepo = TypeFunctionRepo.getInstance();

            public Parser build()
            {
                Parser parser = new Parser();

                SetIndexNameSpace setindexnamespace = new SetIndexNameSpace(typefunctionrepo);
                DetectNamespace detectNS = new DetectNamespace();
                detectNS.add(setindexnamespace);
                parser.add(detectNS);

                SetIndexType setindextype = new SetIndexType(typefunctionrepo);
                AddInheritence addInheritence = new AddInheritence(typefunctionrepo);//---<capturing inheritence relation>----
                DetectClassModified detectCM = new DetectClassModified();

                detectCM.add(setindextype);
                detectCM.add(addInheritence);
                parser.add(detectCM);

                AddComposition addcomposition = new AddComposition(typefunctionrepo);//---<capturing composition in typefunctionrepo>----
                DetectStructModified detectSM = new DetectStructModified();

                detectSM.add(addcomposition);
                parser.add(detectSM);

                AddAggregation aggregation = new AddAggregation(typefunctionrepo);//---<capturing aggregation relation>----
                DetectAggregation detectAg = new DetectAggregation();

                detectAg.add(aggregation);
                parser.add(detectAg);

                AddUsingRel addusingrel = new AddUsingRel(typefunctionrepo);//------<using relation>------
                DetectFunctionModified detectFM = new DetectFunctionModified();

                detectFM.add(addusingrel);
                parser.add(detectFM);

                DetectEnum detectE = new DetectEnum();
                detectE.add(addcomposition);
                parser.add(detectE);

                
                DetectComposition detectC = new DetectComposition();
                detectC.add(addcomposition);
                parser.add(detectC);

                return parser;
            }
        }
    }


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;
using System.IO;

namespace Server
{
    class ProjListXML
    {
        void createXML(string serverAdd)
        {
            string dir = Path.GetFullPath("../../../");
            ProjList projlist = new ProjList();
            List<PathInfo> projects = projlist.getProjectList(serverAdd);

            XDocument doc = new XDocument();
            XElement root = new XElement("projects");
            doc.Add(root);
            foreach (PathInfo project in projects)
            {
                XElement proj = new XElement("project");
                XElement name = new XElement("name");
                name.Value = project.projName;

                proj.Add(name);
                root.Add(proj);

            }
            Console.WriteLine(doc.ToString());
            doc.Save("Project_list.xml");

            Console.WriteLine("present dir:" + dir);
            System.IO.File.WriteAllText(@dir + "Projects.xml", doc.ToString());

        }
#if(TEST_XML)
        public static void Main(string[] st)
        {
            ProjListXML xml=new ProjListXML();
            xml.createXML("F:/Project2");

        }
#endif
    }
}

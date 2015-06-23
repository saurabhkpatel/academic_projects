using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Server
{
    class PathInfo
    {
        String projname_, path_;
        public String projName
        {
            get
            { return projname_; }
            set
            { projname_ = value; }
        }

        public String path
        {
            get
            { return path_; }
            set
            { path_ = value; }

        }
    }
    class ProjList
    {

        public List<PathInfo> getProjectList(string serverAdd)
        {
            List<PathInfo> projects = new List<PathInfo>();

            string path = Path.GetFullPath(serverAdd);
            Directory.SetCurrentDirectory(path);

            string[] files = Directory.GetFiles(path, "*.sln");
            foreach (string file in files)
            {
                Console.WriteLine("testing file {0}", file);
                PathInfo info = new PathInfo();

                info.projName = Path.GetFileName(file);//MakeRelative(path, file); original
                info.path = serverAdd;
                projects.Add(info);
            }
            string[] dirs = Directory.GetDirectories(serverAdd);
            foreach (string dir in dirs)
            {
                projects.AddRange(getProjectList(dir));
            }
            return projects;
        }

        public static string MakeRelative(string fromDirectory, string toPath)
        {
            if (fromDirectory == null)
                throw new ArgumentNullException("fromDirectory");
            if (toPath == null)
                throw new ArgumentNullException("toPath");

            bool isRooted = (Path.IsPathRooted(fromDirectory) && Path.IsPathRooted(toPath));
            if (isRooted)
            {
                bool isDifferentRoot = (string.Compare(Path.GetPathRoot(fromDirectory), Path.GetPathRoot(toPath), true) != 0);
                if (isDifferentRoot)
                    return toPath;
            }

            List<string> relativePath = new List<string>();
            string[] fromDirectories = fromDirectory.Split(Path.DirectorySeparatorChar);
            string[] toDirectories = toPath.Split(Path.DirectorySeparatorChar);

            int length = Math.Min(fromDirectories.Length, toDirectories.Length);

            int lastCommonRoot = -1;

            // find common root
            for (int x = 0; x < length; x++)
            {
                if (string.Compare(fromDirectories[x], toDirectories[x], true) != 0)
                    break;

                lastCommonRoot = x;
            }

            if (lastCommonRoot == -1)
                return toPath;

            // add relative folders in from path
            for (int x = lastCommonRoot + 1; x < fromDirectories.Length; x++)
            {
                if (fromDirectories[x].Length > 0)
                    relativePath.Add("..");
            }

            // add to folders to path
            for (int x = lastCommonRoot + 1; x < toDirectories.Length; x++)
            {
                relativePath.Add(toDirectories[x]);
            }

            // create relative path
            string[] relativeParts = new string[relativePath.Count];
            relativePath.CopyTo(relativeParts, 0);

            string newPath = string.Join(Path.DirectorySeparatorChar.ToString(), relativeParts);

            return newPath;
        }


#if(TEST_PROJ_LIST)
        static void Main(string[] args)
        {
            ProjList projlist = new ProjList();
            List<PathInfo> projects = projlist.getProjectList("F:/Project2");
            foreach(PathInfo path in projects)
            {
                Console.WriteLine(" project name: {0} project path:{1}", path.projName,path.path);
            }

        }
    }
#endif
    }
}
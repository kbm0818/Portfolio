using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;

namespace Viewer
{
    class Helper
    {
        public static void SearchDirectory(ref List<string> list, string dir, string pattern)
        {
            foreach (string file in Directory.GetFiles(dir, pattern))
                list.Add(file);

            foreach (string d in Directory.GetDirectories(dir))
                SearchDirectory(ref list, d, pattern);
        }

        public static string ToString(IntPtr ptr)
        {
            return Marshal.PtrToStringAnsi(ptr);
        }
    }
}

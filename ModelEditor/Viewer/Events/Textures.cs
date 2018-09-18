using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace Viewer
{
    class Textures
    {
        private ListBox _listBox;

        private readonly string _texturePath
            = Path.Combine(Environment.CurrentDirectory, "../../_Contents/Textures/");

        public Textures(ListBox listBox)
        {
            _listBox = listBox;

            RefreshList();
        }

        private void RefreshList()
        {
            List<string> fileList = new List<string>();
            Helper.SearchDirectory(ref fileList, _texturePath, "*.png");

            _listBox.Items.Clear();
            foreach (string temp in fileList)
            {
                FileItem item = new FileItem();
                item.File = Path.GetFileName(temp);
                item.Path = Path.GetFullPath(temp);

                _listBox.Items.Add(item);
            }
        }

        public void Refresh(object sender, EventArgs e)
        {
            RefreshList();
        }
    }
}

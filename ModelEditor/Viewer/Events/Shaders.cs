using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace Viewer
{
    public struct FileItem
    {
        public string File;
        public string Path;

        public override string ToString()
        {
            return File;
        }
    }

    class Shaders
    {
        private ListBox _listBox;
        private ListBox _partsList;
        private TextBox _partsPathTextBox;
        //   private TextBox _shaderFilePathText;

        private readonly string _shaderPath
            = Path.Combine(Environment.CurrentDirectory, "../../_Shaders/");
        private readonly string _partsPath
            = Path.Combine(Environment.CurrentDirectory, "../../_Contents/");

        public Shaders(ListBox listBox, ListBox listBox2, TextBox partsPathTextBox)
        {
            _listBox = listBox;
            _partsList = listBox2;
            _partsPathTextBox = partsPathTextBox;
           // _shaderFilePathText = shaderFilePathText;

            RefreshList();
            RefreshPartsList();

            _partsList.MouseDoubleClick += PartsListBox_DoubleClick;
        }

        private void RefreshList()
        {
            List<string> fileList = new List<string>();
            Helper.SearchDirectory(ref fileList, _shaderPath, "*.hlsl");

            _listBox.Items.Clear();
            foreach(string temp in fileList)
            {
                FileItem item = new FileItem();
                item.File = Path.GetFileName(temp);
                item.Path = Path.GetFullPath(temp);

                _listBox.Items.Add(item);
            }
        }

        private void RefreshPartsList()
        {
            List<string> fileList = new List<string>();
            Helper.SearchDirectory(ref fileList, _partsPath, "*.fbx");

            _partsList.Items.Clear();
            foreach (string temp in fileList)
            {
                FileItem item = new FileItem();
                item.File = Path.GetFileName(temp);
                item.Path = Path.GetFullPath(temp);

                _partsList.Items.Add(item);
            }
        }

        public void Refresh(object sender, EventArgs e)
        {
            RefreshList();
        }
        public void RefreshPartsList(object sender, EventArgs e)
        {
            RefreshPartsList();
        }

        //public void DragEnter(object sender, DragEventArgs e)
        //{
        //    Type type = typeof(FileItem);
        //    if(e.Data.GetDataPresent(type))
        //    {
        //        e.Effect = DragDropEffects.Copy;
        //        FileItem item = (FileItem)e.Data.GetData(type);

        //        _shaderFilePathText.Text = item.File;
        //    }
        //}

        //public void DragDrop(object sender, DragEventArgs e)
        //{
        //    Type type = typeof(FileItem);
        //    if (e.Data.GetDataPresent(type))
        //    {
        //        FileItem item = (FileItem)e.Data.GetData(type);

        //        StringBuilder builder = new StringBuilder(item.Path);
        //        Cs_SetShader(builder);
        //    }
        //}

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_SetShader(StringBuilder file);

        private void PartsListBox_DoubleClick(object sender, EventArgs e)
        {
            if(_partsList.SelectedItem != null)
            {
                _partsPathTextBox.Text = _partsList.SelectedItem.ToString();
            }
        }
    }
}

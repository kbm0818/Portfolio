using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Viewer
{
    class Models
    {
        private string _modelFile = "";
        private TableLayoutPanel _materialTable;
        private TreeView _skeletonTreeView;
        private Panel _skeletonPanel;
        private TrackBar _animationTrackBar;
        private Panel _animationBarPanel;
        private TableLayoutPanel _modelLayoutPanel;
        private TextBox _partsPathTextBox;
        private TextBox _partsSkeletonTextBox;

        public Models(TableLayoutPanel materialTable, TreeView skeletonTreeView, Panel skeletonPanel, TrackBar animationTrackBar, Panel animationBarPanel, TableLayoutPanel modelLayoutPanel, TextBox filePath, TextBox skeletonName)
        {
            _materialTable = materialTable;
            _skeletonTreeView = skeletonTreeView;
            _animationTrackBar = animationTrackBar;
            _skeletonPanel = skeletonPanel;
            _animationBarPanel = animationBarPanel;
            _modelLayoutPanel = modelLayoutPanel;
            _partsPathTextBox = filePath;
            _partsSkeletonTextBox = skeletonName;

            _materialTable.RowStyles[0].SizeType = SizeType.Absolute;
            _materialTable.RowStyles[0].Height = 27;

            _materialTable.ColumnStyles[0].SizeType = SizeType.Percent;
            _materialTable.ColumnStyles[0].Width = 60;

            _materialTable.ColumnStyles[1].SizeType = SizeType.AutoSize;

            _skeletonTreeView.NodeMouseDoubleClick += SkeletonTreeView_DoubleClick;
            _partsPathTextBox.TextChanged += PartsTextBox_Changed;
            _partsSkeletonTextBox.TextChanged += PartsTextBox_Changed;

        }


        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_FbxConvert(string fbxFile, string saveFile);

        public void FileFbxConvertMenu_Click(object sender, EventArgs e)
        {
            string fbx = Path.Combine(Environment.CurrentDirectory, "../../_Contents/FbxModels");

            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = Path.GetFullPath(fbx);
            ofd.Filter = "FBX Files (*.fbx)|*.fbx|Obj Files (*.obj)|*.obj";
            ofd.RestoreDirectory = true;
            ofd.Title = "FBX 파일 열기";

            string fbxFile = "";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                fbxFile = ofd.FileName;


                string bin = Path.Combine(Environment.CurrentDirectory, "../../_Contents/BinModels");

                SaveFileDialog sfd = new SaveFileDialog();
                sfd.InitialDirectory = Path.GetFullPath(bin);
                sfd.Filter = "Model File(*.model)|*.model";
                sfd.RestoreDirectory = true;
                sfd.FileName = Path.GetFileNameWithoutExtension(fbxFile);
                sfd.Title = "Binary Model 저장";

                if(sfd.ShowDialog() == DialogResult.OK)
                {
                    Cs_FbxConvert(fbxFile, sfd.FileName);
                }
            }//if(ofd)
        }


        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_BinModelLoad(string binFile);

        public void FileOpenMenu_Click(object sender, EventArgs e)
        {
            string bin = Path.Combine(Environment.CurrentDirectory, "../../_Contents/BinModels");

            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = Path.GetFullPath(bin);
            ofd.Filter = "Model Files (*.model)|*.model";
            ofd.RestoreDirectory = true;
            ofd.Title = "Model 파일 열기";

            if(ofd.ShowDialog() == DialogResult.OK)
            {
                Cs_BinModelLoad(ofd.FileName);
                AddMaterialTable();
                SkeletonTreeView_Refresh(); // 스켈레톤
                AddPartTable();

                _modelFile = ofd.SafeFileName;
            }
        }

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_BinModelSave(string binFile);

        public void FileSaveMenu_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(_modelFile) == true) return;


            string bin = Path.Combine(Environment.CurrentDirectory, "../../_Contents/BinModels");

            SaveFileDialog sfd = new SaveFileDialog();
            sfd.InitialDirectory = Path.GetFullPath(bin);
            sfd.Filter = "Model File(*.model)|*.model";
            sfd.RestoreDirectory = true;
            sfd.FileName = Path.GetFileNameWithoutExtension(_modelFile);
            sfd.Title = "Binary Model 저장";

            if (sfd.ShowDialog() == DialogResult.OK)
            {
                Cs_BinModelSave(sfd.FileName);
                _modelFile = Path.GetFileName(sfd.FileName);
            }
        }


        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern uint Cs_GetMaterialCount();

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Cs_GetMaterialDiffuseTextureFile(int number);

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Cs_GetMaterialName(int number);

        private List<TextBox> _materialList = new List<TextBox>();
        private List<FileItem> _fileList = new List<FileItem>();

        private void AddMaterialTable()
        {
            _materialList.Clear();
            _fileList.Clear();
            for (int i = _materialTable.Controls.Count - 1; i >= 0; i--)
                _materialTable.Controls.RemoveAt(i);

            int count = (int)Cs_GetMaterialCount();
            _materialTable.RowCount = count + 1;

            for(int i = 0; i < count; i++)
            {
                string materialName = Helper.ToString(Cs_GetMaterialName(i));
                //Marshal.FreeHGlobal(materialPtr);

                Label label = new Label();
                label.Dock = DockStyle.Fill;
                label.Text = materialName;
                label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
                _materialTable.Controls.Add(label);

                string diffuseTexture = Helper.ToString(Cs_GetMaterialDiffuseTextureFile(i));

                FileItem item = new FileItem();
                if(String.IsNullOrEmpty(diffuseTexture) == true)
                {
                    item.File = "";
                    item.Path = "";
                }
                else
                {
                    item.File = Path.GetFileName(diffuseTexture);
                    item.Path = Path.GetFullPath(diffuseTexture);
                }

                TextBox textBox = new TextBox();
                textBox.Text = item.File;
                textBox.Dock = DockStyle.Fill;
                textBox.Tag = i;
                textBox.AllowDrop = true;
                textBox.DragEnter += TextBox_DragEnter;
                textBox.DragDrop += TextBox_DragDrop;

                _materialList.Add(textBox);
                _materialTable.Controls.Add(textBox);
            }
        }

        private void TextBox_DragEnter(object sender, DragEventArgs e)
        {
            Type type = typeof(FileItem);

            if (e.Data.GetDataPresent(type))
                e.Effect = DragDropEffects.Copy;
        }


        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_SetMaterialDiffuseTextureFile(int number, string file);

        private void TextBox_DragDrop(object sender, DragEventArgs e)
        {
            Type type = typeof(FileItem);
            if (e.Data.GetDataPresent(type))
            {
                FileItem item = (FileItem)e.Data.GetData(type);

                TextBox textBox = (TextBox)sender;
                textBox.Text = item.File;
                int number = (int)textBox.Tag;

                Cs_SetMaterialDiffuseTextureFile(number, item.Path);
            }
        }

        private void SkeletonTreeView_Refresh()
        {
            _skeletonTreeView.Nodes.Clear();

            int skeletonSize = Cs_GetModelSkeletonSize();
            if (skeletonSize < 1)
            {
                _skeletonPanel.Visible = false;
                return;
            }
            _skeletonPanel.Visible = true;

            ImageList imgList = new ImageList();
            imgList.Images.Add(Bitmap.FromFile(System.Environment.GetFolderPath  
            (System.Environment.SpecialFolder.Personal) + @"\Skeleton.png")); 
            _skeletonTreeView.ImageList = imgList;

            // 첫번째 TreeView 아이템 - 서버
            TreeNode skeletonNode = new TreeNode("Skeleton", 0, 0);

            for(int i = 0; i < skeletonSize; i++)
            {
                string boneName = Helper.ToString(Cs_GetModelSkeletonName(i));
                skeletonNode.Nodes.Add(i.ToString(), boneName, 0, 0);
            }
       
            //노드를 트리뷰에 더해줌
            _skeletonTreeView.Nodes.Add(skeletonNode);

            //모든 트리를 보여주는 상태로
            _skeletonTreeView.ExpandAll();
        }

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Cs_GetModelSkeletonName(int index);

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int Cs_GetModelSkeletonSize();

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern uint Cs_GetModelPartSize();

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Cs_GetModelPartName(int number);

        private List<Label> _partList = new List<Label>();
        private List<Button> _buttonList = new List<Button>();

        private void AddPartTable()
        {
            _partList.Clear();
            _buttonList.Clear();
            _modelLayoutPanel.Controls.Clear();
            
            int count = (int)Cs_GetModelPartSize();
            _modelLayoutPanel.RowCount = count + 1;
         
            for (int i = 0; i < count; i++)
            {
                string partName = Helper.ToString(Cs_GetModelPartName(i));

                Label label = new Label();
                label.Text = partName;
                label.Dock = DockStyle.Fill;
                _partList.Add(label);

                Button button = new Button();
                button.Text = "Delete";
                button.Dock = DockStyle.Fill;
                button.Click += DeleteModelPart;

                _buttonList.Add(button);

                _modelLayoutPanel.Controls.Add(label, 0, i);
                _modelLayoutPanel.Controls.Add(button, 1, i);
            }
        }

        private void DeleteModelPart(object sender, EventArgs e)
        {
            for(int i = 0; i < _buttonList.Count; i++)
            {
                if (_buttonList[i].Equals(sender))
                {
                    int result = Cs_DeleteModelPart(i);
                    if (result == 1)
                        AddPartTable();
                    break;
                }
            }
        }

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int Cs_DeleteModelPart(int number);

        public void SkeletonTreeView_DoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            _partsSkeletonTextBox.Text = e.Node.Text;
        }

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_PartAdd(string path, string skeletonName);

        public void PartsTextBox_Changed(object sender, EventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(_partsSkeletonTextBox.Text) && !string.IsNullOrWhiteSpace(_partsPathTextBox.Text))
            {
                string path = Path.Combine(Environment.CurrentDirectory, "../../_Contents/Objects/");
                Cs_PartAdd(Path.GetFullPath(path) + _partsPathTextBox.Text.ToString() , _partsSkeletonTextBox.Text.ToString());
                
                _partsSkeletonTextBox.Text = string.Empty;
                _partsPathTextBox.Text = string.Empty;

                AddPartTable();
            }
        }
    }
}

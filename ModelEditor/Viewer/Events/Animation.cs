using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace Viewer
{
    class Animation
    {
        private ListBox _listBox;
        private TextBox _animationPathTextBox;
        private TextBox _animationNameBox;
        private Button _animationAdderButton;
        private TableLayoutPanel _animationLayoutPanel;
        private TrackBar _aniTrackBar;
        private Panel _aniBarPanel;
        private Timer _timer;

        private readonly string _animationPath
            = Path.Combine(Environment.CurrentDirectory, "../../_Contents/Animations");

        public Animation(ListBox listBox, TextBox aniPathTextBox, TextBox aniNameTextBox, Button aniAdderButton, TableLayoutPanel table, TrackBar trackBar, Panel barPanel)
        {
            _listBox = listBox;
            _animationPathTextBox = aniPathTextBox;
            _animationNameBox = aniNameTextBox;
            _animationAdderButton = aniAdderButton;
            _animationLayoutPanel = table;
            _aniTrackBar = trackBar;
            _aniBarPanel = barPanel;

            RefreshList();

            _listBox.DoubleClick += AnimationListBox_DoubleClick;
            _animationAdderButton.Click += AniAdderButton_Click;

            _timer = new Timer();
            _timer.Interval = 10; // 0.01초
            _timer.Tick += AniTrackBar_Update;
            _timer.Start();
        }
   
        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_AnimationAdd(string path, string animName);

        private void AniAdderButton_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(_animationPathTextBox.Text) && !string.IsNullOrWhiteSpace(_animationNameBox.Text))
            {
                string path = Path.Combine(Environment.CurrentDirectory, "../../_Contents/Animations/");
                Cs_AnimationAdd(Path.GetFullPath(path) + _animationPathTextBox.Text.ToString(), _animationNameBox.Text.ToString());

                _animationPathTextBox.Text = string.Empty;
                _animationNameBox.Text = string.Empty;
                
                RefreshAnimLayoutPanel();
            }
        }

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Cs_GetAnimationName(int number);

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int Cs_GetAnimationSize();

        private List<Label> _nameList = new List<Label>();
        private List<Button> _buttonList = new List<Button>();
        private List<Button> _playButtonList = new List<Button>();


        private void RefreshAnimLayoutPanel()
        {
            _nameList.Clear();
            _buttonList.Clear();
            _playButtonList.Clear();

            _animationLayoutPanel.Controls.Clear();

            int count = (int)Cs_GetAnimationSize();
            if (count > 0)
                _aniBarPanel.Visible = true;
            else
            {
                _aniBarPanel.Visible = false;
                _timer.Stop();
            }

            _animationLayoutPanel.RowCount = count + 1;

            for (int i = 0; i < count; i++)
            {
                string animName = Helper.ToString(Cs_GetAnimationName(i));

                Label label = new Label();
                label.Text = animName;
                label.Dock = DockStyle.Fill;
                label.Font  = new Font("Arial", 20);
                _nameList.Add(label);

                Button button = new Button();
                button.Text = "Delete";
                button.Dock = DockStyle.Fill;
                button.Click += DeleteAnim;
                _buttonList.Add(button);

                Button button2 = new Button();
                button2.Text = "Play";
                button2.Dock = DockStyle.Fill;
                button2.Click += PlayAnim;
                _playButtonList.Add(button2);
                
                _animationLayoutPanel.Controls.Add(label, 0, i);
                _animationLayoutPanel.Controls.Add(button2, 1, i);
                _animationLayoutPanel.Controls.Add(button, 2, i);
            }
        }

        private void AnimationListBox_DoubleClick(object sender, EventArgs e)
        {
            if (_listBox.SelectedItem != null)
            {
                _animationPathTextBox.Text = _listBox.SelectedItem.ToString();
            }
        }

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int Cs_PlayAnimation(int number);

        private void PlayAnim(object sender, EventArgs e)
        {
            for (int i = 0; i < _playButtonList.Count; i++)
            {
                _nameList[i].ForeColor = System.Drawing.Color.Black;

                if (_playButtonList[i].Equals(sender))
                {
                    Cs_PlayAnimation(i); 
                    _nameList[i].ForeColor = System.Drawing.Color.Red;
                }
            }
        }
        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern float Cs_KeyFrameFactor();
        
        public void AniTrackBar_Update(object sender, EventArgs e)
        {
            if (_animationLayoutPanel.Controls.Count > 0)
            {
                float temp = Cs_KeyFrameFactor() * 1000.0f;
          
                _aniTrackBar.Value = (int)temp;
                _aniTrackBar.Refresh();
            }
        }

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_DeleteAnimation(int number);

        private void DeleteAnim(object sender, EventArgs e)
        {
            for (int i = 0; i < _buttonList.Count; i++)
            {
                if (_buttonList[i].Equals(sender))
                {
                    Cs_DeleteAnimation(i);
                    RefreshAnimLayoutPanel();
                    break;
                }
            }
        }

        private void RefreshList()
        {
            List<string> fileList = new List<string>();
            Helper.SearchDirectory(ref fileList, _animationPath, "*.fbx");

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

        public void DragEnter(object sender, DragEventArgs e)
        {
            Type type = typeof(FileItem);
            if (e.Data.GetDataPresent(type))
            {
                e.Effect = DragDropEffects.Copy;
                FileItem item = (FileItem)e.Data.GetData(type);

                //_shaderFilePathText.Text = item.File;
            }
        }

        public void DragDrop(object sender, DragEventArgs e)
        {
            Type type = typeof(FileItem);
            if (e.Data.GetDataPresent(type))
            {
                FileItem item = (FileItem)e.Data.GetData(type);

               // StringBuilder builder = new StringBuilder(item.Path);
               // Cs_SetShader(builder);
            }
        }
    }
}

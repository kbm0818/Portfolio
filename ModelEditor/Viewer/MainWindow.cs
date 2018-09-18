using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Viewer
{
    public partial class MainForm : Form
    {
        private Shaders _shaders;
        private Textures _textures;
        private Models _models;
        private Animation _animations;

        public MainForm()
        {
            InitializeComponent();

            LogView.ListView = this.LogListView;
            Connect.RenderBox = this.RenderBox;

            this.Load += Connect.Create;
            this.FormClosed += Connect.Destory;
            Application.Idle += Connect.Idle;

            _models = new Models(MaterialTable, SkeletonTreeView, SkeletonPanel, AnimationTrackBar, AnimationBarPanel, ModelLayoutPanel, ModelAdderPathBox, ModelAdderSkeletonBox);
            FileFbxConvertMenu.Click += _models.FileFbxConvertMenu_Click;
            FileOpenMenu.Click += _models.FileOpenMenu_Click;
            FileSaveMenu.Click += _models.FileSaveMenu_Click;

            _shaders = new Shaders(ShaderFileList, PartsListBox, ModelAdderPathBox);
            RefreshShaderFileList.Click += _shaders.Refresh;
            ShaderFileList.MouseDown += ShaderFileLlist_MouseDown;

            _textures = new Textures(TextureFileList);
            TextureFileList.MouseDown += TextureFileList_MouseDown;
            RefreshTextureFileList.Click += _textures.Refresh;

            _animations = new Animation(AnimationFileList, AnimationFilePathTextBox,AnimationNameTextBox, AnimationAdderButton, AnimationLayoutPanel, AnimationTrackBar, AnimationBarPanel);
            RefreshAnimationFileList.Click += _animations.Refresh;

            FrameTimer.Tick += FrameTimer_Tick;
           
            LogListView.Focus();
        }


        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern uint Cs_GetFrame();

        private void FrameTimer_Tick(object sender, EventArgs e)
        {
            StringBuilder builder = new StringBuilder();
            builder.Append("FPS : ");
            builder.Append(Cs_GetFrame());

            FrameStrip.Text = builder.ToString() ;
        }

        private void ShaderFileLlist_MouseDown(object sender, MouseEventArgs e)
        {
            FileItem item = (FileItem)ShaderFileList.SelectedItem;

            DoDragDrop(item, DragDropEffects.Copy);
        }

        private void TextureFileList_MouseDown(object sender, MouseEventArgs e)
        {
            FileItem item = (FileItem)TextureFileList.SelectedItem;

            DoDragDrop(item, DragDropEffects.Copy);
        }
    }
}

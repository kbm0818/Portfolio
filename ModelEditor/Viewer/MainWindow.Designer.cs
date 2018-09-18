namespace Viewer
{
    partial class MainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.MainPanel = new System.Windows.Forms.Panel();
            this.AnimationBarPanel = new System.Windows.Forms.Panel();
            this.AnimationTrackBar = new System.Windows.Forms.TrackBar();
            this.SkeletonPanel = new System.Windows.Forms.Panel();
            this.SkeletonTreeView = new System.Windows.Forms.TreeView();
            this.BottomPanel = new System.Windows.Forms.Panel();
            this.LogListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.RightPanel = new System.Windows.Forms.Panel();
            this.RightSplitContainer = new System.Windows.Forms.SplitContainer();
            this.InspectorTab = new System.Windows.Forms.TabControl();
            this.MaterialPage = new System.Windows.Forms.TabPage();
            this.MaterialTable = new System.Windows.Forms.TableLayoutPanel();
            this.ModelPage = new System.Windows.Forms.TabPage();
            this.ModelPartAdderPanel = new System.Windows.Forms.Panel();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.ModelAdderSkeletonBox = new System.Windows.Forms.TextBox();
            this.ModelAdderPathBox = new System.Windows.Forms.TextBox();
            this.ModelLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.AniPage = new System.Windows.Forms.TabPage();
            this.AnimationLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.AnimationNameTextBox = new System.Windows.Forms.TextBox();
            this.AnimationFilePathTextBox = new System.Windows.Forms.TextBox();
            this.AnimationAdderButton = new System.Windows.Forms.Button();
            this.RightSplitter = new System.Windows.Forms.Splitter();
            this.ProjectTab = new System.Windows.Forms.TabControl();
            this.TexturePage = new System.Windows.Forms.TabPage();
            this.RefreshTextureFileList = new System.Windows.Forms.Button();
            this.TextureFileList = new System.Windows.Forms.ListBox();
            this.ShaderPage = new System.Windows.Forms.TabPage();
            this.RefreshShaderFileList = new System.Windows.Forms.Button();
            this.ShaderFileList = new System.Windows.Forms.ListBox();
            this.AnimationPage = new System.Windows.Forms.TabPage();
            this.RefreshAnimationFileList = new System.Windows.Forms.Button();
            this.AnimationFileList = new System.Windows.Forms.ListBox();
            this.PartsPage = new System.Windows.Forms.TabPage();
            this.PartsListBox = new System.Windows.Forms.ListBox();
            this.RefreshPartsListButton = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.FrameStrip = new System.Windows.Forms.ToolStripStatusLabel();
            this.RenderBox = new System.Windows.Forms.PictureBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.FileFbxConvertMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.FileOpenMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.FileSaveMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.FileExitMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.FrameTimer = new System.Windows.Forms.Timer(this.components);
            this.MainPanel.SuspendLayout();
            this.AnimationBarPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.AnimationTrackBar)).BeginInit();
            this.SkeletonPanel.SuspendLayout();
            this.BottomPanel.SuspendLayout();
            this.RightPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RightSplitContainer)).BeginInit();
            this.RightSplitContainer.Panel1.SuspendLayout();
            this.RightSplitContainer.Panel2.SuspendLayout();
            this.RightSplitContainer.SuspendLayout();
            this.InspectorTab.SuspendLayout();
            this.MaterialPage.SuspendLayout();
            this.ModelPage.SuspendLayout();
            this.ModelPartAdderPanel.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.AniPage.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.ProjectTab.SuspendLayout();
            this.TexturePage.SuspendLayout();
            this.ShaderPage.SuspendLayout();
            this.AnimationPage.SuspendLayout();
            this.PartsPage.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RenderBox)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainPanel
            // 
            this.MainPanel.Controls.Add(this.AnimationBarPanel);
            this.MainPanel.Controls.Add(this.SkeletonPanel);
            this.MainPanel.Controls.Add(this.BottomPanel);
            this.MainPanel.Controls.Add(this.RightPanel);
            this.MainPanel.Controls.Add(this.statusStrip1);
            this.MainPanel.Controls.Add(this.RenderBox);
            this.MainPanel.Controls.Add(this.menuStrip1);
            this.MainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainPanel.Location = new System.Drawing.Point(0, 0);
            this.MainPanel.Margin = new System.Windows.Forms.Padding(1);
            this.MainPanel.Name = "MainPanel";
            this.MainPanel.Size = new System.Drawing.Size(835, 690);
            this.MainPanel.TabIndex = 2;
            // 
            // AnimationBarPanel
            // 
            this.AnimationBarPanel.Controls.Add(this.AnimationTrackBar);
            this.AnimationBarPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.AnimationBarPanel.Location = new System.Drawing.Point(200, 470);
            this.AnimationBarPanel.Name = "AnimationBarPanel";
            this.AnimationBarPanel.Size = new System.Drawing.Size(337, 32);
            this.AnimationBarPanel.TabIndex = 6;
            this.AnimationBarPanel.Visible = false;
            // 
            // AnimationTrackBar
            // 
            this.AnimationTrackBar.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AnimationTrackBar.Location = new System.Drawing.Point(0, 0);
            this.AnimationTrackBar.Maximum = 1000;
            this.AnimationTrackBar.Name = "AnimationTrackBar";
            this.AnimationTrackBar.Size = new System.Drawing.Size(337, 32);
            this.AnimationTrackBar.SmallChange = 10;
            this.AnimationTrackBar.TabIndex = 0;
            this.AnimationTrackBar.TickFrequency = 100;
            // 
            // SkeletonPanel
            // 
            this.SkeletonPanel.Controls.Add(this.SkeletonTreeView);
            this.SkeletonPanel.Dock = System.Windows.Forms.DockStyle.Left;
            this.SkeletonPanel.Location = new System.Drawing.Point(0, 24);
            this.SkeletonPanel.Name = "SkeletonPanel";
            this.SkeletonPanel.Size = new System.Drawing.Size(200, 478);
            this.SkeletonPanel.TabIndex = 5;
            this.SkeletonPanel.Visible = false;
            // 
            // SkeletonTreeView
            // 
            this.SkeletonTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SkeletonTreeView.Location = new System.Drawing.Point(0, 0);
            this.SkeletonTreeView.Name = "SkeletonTreeView";
            this.SkeletonTreeView.Size = new System.Drawing.Size(200, 478);
            this.SkeletonTreeView.TabIndex = 0;
            // 
            // BottomPanel
            // 
            this.BottomPanel.Controls.Add(this.LogListView);
            this.BottomPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.BottomPanel.Location = new System.Drawing.Point(0, 502);
            this.BottomPanel.Margin = new System.Windows.Forms.Padding(1);
            this.BottomPanel.Name = "BottomPanel";
            this.BottomPanel.Size = new System.Drawing.Size(537, 166);
            this.BottomPanel.TabIndex = 4;
            // 
            // LogListView
            // 
            this.LogListView.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.LogListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.LogListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LogListView.GridLines = true;
            this.LogListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.LogListView.Location = new System.Drawing.Point(0, 0);
            this.LogListView.Margin = new System.Windows.Forms.Padding(1);
            this.LogListView.Name = "LogListView";
            this.LogListView.Size = new System.Drawing.Size(537, 166);
            this.LogListView.Sorting = System.Windows.Forms.SortOrder.Descending;
            this.LogListView.TabIndex = 0;
            this.LogListView.UseCompatibleStateImageBehavior = false;
            this.LogListView.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Width = 80;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Width = 700;
            // 
            // RightPanel
            // 
            this.RightPanel.Controls.Add(this.RightSplitContainer);
            this.RightPanel.Dock = System.Windows.Forms.DockStyle.Right;
            this.RightPanel.Location = new System.Drawing.Point(537, 24);
            this.RightPanel.Margin = new System.Windows.Forms.Padding(1);
            this.RightPanel.Name = "RightPanel";
            this.RightPanel.Size = new System.Drawing.Size(298, 644);
            this.RightPanel.TabIndex = 3;
            // 
            // RightSplitContainer
            // 
            this.RightSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.RightSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.RightSplitContainer.Margin = new System.Windows.Forms.Padding(1);
            this.RightSplitContainer.Name = "RightSplitContainer";
            this.RightSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // RightSplitContainer.Panel1
            // 
            this.RightSplitContainer.Panel1.Controls.Add(this.InspectorTab);
            this.RightSplitContainer.Panel1.Controls.Add(this.RightSplitter);
            // 
            // RightSplitContainer.Panel2
            // 
            this.RightSplitContainer.Panel2.Controls.Add(this.ProjectTab);
            this.RightSplitContainer.Size = new System.Drawing.Size(298, 644);
            this.RightSplitContainer.SplitterDistance = 185;
            this.RightSplitContainer.SplitterWidth = 2;
            this.RightSplitContainer.TabIndex = 0;
            // 
            // InspectorTab
            // 
            this.InspectorTab.Controls.Add(this.MaterialPage);
            this.InspectorTab.Controls.Add(this.ModelPage);
            this.InspectorTab.Controls.Add(this.AniPage);
            this.InspectorTab.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorTab.Location = new System.Drawing.Point(0, 0);
            this.InspectorTab.Margin = new System.Windows.Forms.Padding(1);
            this.InspectorTab.Name = "InspectorTab";
            this.InspectorTab.SelectedIndex = 0;
            this.InspectorTab.Size = new System.Drawing.Size(298, 180);
            this.InspectorTab.TabIndex = 1;
            // 
            // MaterialPage
            // 
            this.MaterialPage.Controls.Add(this.MaterialTable);
            this.MaterialPage.Location = new System.Drawing.Point(4, 22);
            this.MaterialPage.Margin = new System.Windows.Forms.Padding(1);
            this.MaterialPage.Name = "MaterialPage";
            this.MaterialPage.Size = new System.Drawing.Size(290, 154);
            this.MaterialPage.TabIndex = 2;
            this.MaterialPage.Text = "Material";
            this.MaterialPage.UseVisualStyleBackColor = true;
            // 
            // MaterialTable
            // 
            this.MaterialTable.AutoScroll = true;
            this.MaterialTable.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.MaterialTable.ColumnCount = 2;
            this.MaterialTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.MaterialTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 19F));
            this.MaterialTable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MaterialTable.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
            this.MaterialTable.Location = new System.Drawing.Point(0, 0);
            this.MaterialTable.Margin = new System.Windows.Forms.Padding(1);
            this.MaterialTable.Name = "MaterialTable";
            this.MaterialTable.RowCount = 1;
            this.MaterialTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.MaterialTable.Size = new System.Drawing.Size(290, 154);
            this.MaterialTable.TabIndex = 0;
            // 
            // ModelPage
            // 
            this.ModelPage.Controls.Add(this.ModelPartAdderPanel);
            this.ModelPage.Controls.Add(this.ModelLayoutPanel);
            this.ModelPage.Location = new System.Drawing.Point(4, 22);
            this.ModelPage.Margin = new System.Windows.Forms.Padding(1);
            this.ModelPage.Name = "ModelPage";
            this.ModelPage.Size = new System.Drawing.Size(290, 154);
            this.ModelPage.TabIndex = 1;
            this.ModelPage.Text = "Model";
            this.ModelPage.UseVisualStyleBackColor = true;
            // 
            // ModelPartAdderPanel
            // 
            this.ModelPartAdderPanel.Controls.Add(this.tableLayoutPanel1);
            this.ModelPartAdderPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.ModelPartAdderPanel.Location = new System.Drawing.Point(0, 132);
            this.ModelPartAdderPanel.Name = "ModelPartAdderPanel";
            this.ModelPartAdderPanel.Size = new System.Drawing.Size(290, 22);
            this.ModelPartAdderPanel.TabIndex = 7;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.ModelAdderSkeletonBox, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.ModelAdderPathBox, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(290, 22);
            this.tableLayoutPanel1.TabIndex = 7;
            // 
            // ModelAdderSkeletonBox
            // 
            this.ModelAdderSkeletonBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ModelAdderSkeletonBox.Location = new System.Drawing.Point(148, 3);
            this.ModelAdderSkeletonBox.Name = "ModelAdderSkeletonBox";
            this.ModelAdderSkeletonBox.Size = new System.Drawing.Size(139, 21);
            this.ModelAdderSkeletonBox.TabIndex = 8;
            // 
            // ModelAdderPathBox
            // 
            this.ModelAdderPathBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ModelAdderPathBox.Location = new System.Drawing.Point(3, 3);
            this.ModelAdderPathBox.Name = "ModelAdderPathBox";
            this.ModelAdderPathBox.Size = new System.Drawing.Size(139, 21);
            this.ModelAdderPathBox.TabIndex = 0;
            // 
            // ModelLayoutPanel
            // 
            this.ModelLayoutPanel.AutoScroll = true;
            this.ModelLayoutPanel.AutoSize = true;
            this.ModelLayoutPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ModelLayoutPanel.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.ModelLayoutPanel.ColumnCount = 2;
            this.ModelLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 70F));
            this.ModelLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.ModelLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ModelLayoutPanel.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
            this.ModelLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.ModelLayoutPanel.Margin = new System.Windows.Forms.Padding(1);
            this.ModelLayoutPanel.Name = "ModelLayoutPanel";
            this.ModelLayoutPanel.RowCount = 1;
            this.ModelLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.ModelLayoutPanel.Size = new System.Drawing.Size(290, 154);
            this.ModelLayoutPanel.TabIndex = 1;
            // 
            // AniPage
            // 
            this.AniPage.Controls.Add(this.AnimationLayoutPanel);
            this.AniPage.Controls.Add(this.panel1);
            this.AniPage.Location = new System.Drawing.Point(4, 22);
            this.AniPage.Margin = new System.Windows.Forms.Padding(1);
            this.AniPage.Name = "AniPage";
            this.AniPage.Padding = new System.Windows.Forms.Padding(1);
            this.AniPage.Size = new System.Drawing.Size(290, 154);
            this.AniPage.TabIndex = 0;
            this.AniPage.Text = "Animation";
            this.AniPage.UseVisualStyleBackColor = true;
            // 
            // AnimationLayoutPanel
            // 
            this.AnimationLayoutPanel.AutoScroll = true;
            this.AnimationLayoutPanel.AutoSize = true;
            this.AnimationLayoutPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.AnimationLayoutPanel.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.AnimationLayoutPanel.ColumnCount = 3;
            this.AnimationLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40F));
            this.AnimationLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.AnimationLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.AnimationLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AnimationLayoutPanel.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
            this.AnimationLayoutPanel.Location = new System.Drawing.Point(1, 1);
            this.AnimationLayoutPanel.Margin = new System.Windows.Forms.Padding(1);
            this.AnimationLayoutPanel.Name = "AnimationLayoutPanel";
            this.AnimationLayoutPanel.RowCount = 1;
            this.AnimationLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.AnimationLayoutPanel.Size = new System.Drawing.Size(288, 125);
            this.AnimationLayoutPanel.TabIndex = 9;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.tableLayoutPanel2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(1, 126);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(288, 27);
            this.panel1.TabIndex = 8;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.tableLayoutPanel2.Controls.Add(this.AnimationNameTextBox, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.AnimationFilePathTextBox, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.AnimationAdderButton, 2, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(288, 27);
            this.tableLayoutPanel2.TabIndex = 7;
            // 
            // AnimationNameTextBox
            // 
            this.AnimationNameTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AnimationNameTextBox.Location = new System.Drawing.Point(147, 3);
            this.AnimationNameTextBox.Name = "AnimationNameTextBox";
            this.AnimationNameTextBox.Size = new System.Drawing.Size(80, 21);
            this.AnimationNameTextBox.TabIndex = 8;
            // 
            // AnimationFilePathTextBox
            // 
            this.AnimationFilePathTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AnimationFilePathTextBox.Location = new System.Drawing.Point(3, 3);
            this.AnimationFilePathTextBox.Name = "AnimationFilePathTextBox";
            this.AnimationFilePathTextBox.Size = new System.Drawing.Size(138, 21);
            this.AnimationFilePathTextBox.TabIndex = 0;
            // 
            // AnimationAdderButton
            // 
            this.AnimationAdderButton.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AnimationAdderButton.Location = new System.Drawing.Point(233, 3);
            this.AnimationAdderButton.Name = "AnimationAdderButton";
            this.AnimationAdderButton.Size = new System.Drawing.Size(52, 21);
            this.AnimationAdderButton.TabIndex = 9;
            this.AnimationAdderButton.Text = "Add";
            this.AnimationAdderButton.UseVisualStyleBackColor = true;
            // 
            // RightSplitter
            // 
            this.RightSplitter.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.RightSplitter.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.RightSplitter.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.RightSplitter.Location = new System.Drawing.Point(0, 180);
            this.RightSplitter.Margin = new System.Windows.Forms.Padding(1);
            this.RightSplitter.MinSize = 50;
            this.RightSplitter.Name = "RightSplitter";
            this.RightSplitter.Size = new System.Drawing.Size(298, 5);
            this.RightSplitter.TabIndex = 0;
            this.RightSplitter.TabStop = false;
            // 
            // ProjectTab
            // 
            this.ProjectTab.Controls.Add(this.TexturePage);
            this.ProjectTab.Controls.Add(this.ShaderPage);
            this.ProjectTab.Controls.Add(this.AnimationPage);
            this.ProjectTab.Controls.Add(this.PartsPage);
            this.ProjectTab.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ProjectTab.Location = new System.Drawing.Point(0, 0);
            this.ProjectTab.Margin = new System.Windows.Forms.Padding(1);
            this.ProjectTab.Name = "ProjectTab";
            this.ProjectTab.SelectedIndex = 0;
            this.ProjectTab.Size = new System.Drawing.Size(298, 457);
            this.ProjectTab.TabIndex = 0;
            // 
            // TexturePage
            // 
            this.TexturePage.Controls.Add(this.RefreshTextureFileList);
            this.TexturePage.Controls.Add(this.TextureFileList);
            this.TexturePage.Location = new System.Drawing.Point(4, 22);
            this.TexturePage.Margin = new System.Windows.Forms.Padding(1);
            this.TexturePage.Name = "TexturePage";
            this.TexturePage.Size = new System.Drawing.Size(290, 431);
            this.TexturePage.TabIndex = 1;
            this.TexturePage.Text = "Textures";
            this.TexturePage.UseVisualStyleBackColor = true;
            // 
            // RefreshTextureFileList
            // 
            this.RefreshTextureFileList.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.RefreshTextureFileList.Location = new System.Drawing.Point(0, 399);
            this.RefreshTextureFileList.Margin = new System.Windows.Forms.Padding(1);
            this.RefreshTextureFileList.Name = "RefreshTextureFileList";
            this.RefreshTextureFileList.Size = new System.Drawing.Size(290, 32);
            this.RefreshTextureFileList.TabIndex = 1;
            this.RefreshTextureFileList.Text = "새로고침";
            this.RefreshTextureFileList.UseVisualStyleBackColor = true;
            // 
            // TextureFileList
            // 
            this.TextureFileList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.TextureFileList.FormattingEnabled = true;
            this.TextureFileList.ItemHeight = 12;
            this.TextureFileList.Location = new System.Drawing.Point(2, 2);
            this.TextureFileList.Margin = new System.Windows.Forms.Padding(1);
            this.TextureFileList.Name = "TextureFileList";
            this.TextureFileList.Size = new System.Drawing.Size(291, 376);
            this.TextureFileList.TabIndex = 0;
            // 
            // ShaderPage
            // 
            this.ShaderPage.Controls.Add(this.RefreshShaderFileList);
            this.ShaderPage.Controls.Add(this.ShaderFileList);
            this.ShaderPage.Location = new System.Drawing.Point(4, 22);
            this.ShaderPage.Margin = new System.Windows.Forms.Padding(1);
            this.ShaderPage.Name = "ShaderPage";
            this.ShaderPage.Padding = new System.Windows.Forms.Padding(1);
            this.ShaderPage.Size = new System.Drawing.Size(290, 431);
            this.ShaderPage.TabIndex = 0;
            this.ShaderPage.Text = "Shader";
            this.ShaderPage.UseVisualStyleBackColor = true;
            // 
            // RefreshShaderFileList
            // 
            this.RefreshShaderFileList.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.RefreshShaderFileList.Location = new System.Drawing.Point(1, 398);
            this.RefreshShaderFileList.Margin = new System.Windows.Forms.Padding(1);
            this.RefreshShaderFileList.Name = "RefreshShaderFileList";
            this.RefreshShaderFileList.Size = new System.Drawing.Size(288, 32);
            this.RefreshShaderFileList.TabIndex = 1;
            this.RefreshShaderFileList.Text = "새로고침";
            this.RefreshShaderFileList.UseVisualStyleBackColor = true;
            // 
            // ShaderFileList
            // 
            this.ShaderFileList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.ShaderFileList.FormattingEnabled = true;
            this.ShaderFileList.ItemHeight = 12;
            this.ShaderFileList.Location = new System.Drawing.Point(2, 2);
            this.ShaderFileList.Margin = new System.Windows.Forms.Padding(1);
            this.ShaderFileList.Name = "ShaderFileList";
            this.ShaderFileList.Size = new System.Drawing.Size(288, 376);
            this.ShaderFileList.TabIndex = 0;
            // 
            // AnimationPage
            // 
            this.AnimationPage.Controls.Add(this.RefreshAnimationFileList);
            this.AnimationPage.Controls.Add(this.AnimationFileList);
            this.AnimationPage.Location = new System.Drawing.Point(4, 22);
            this.AnimationPage.Name = "AnimationPage";
            this.AnimationPage.Size = new System.Drawing.Size(290, 431);
            this.AnimationPage.TabIndex = 2;
            this.AnimationPage.Text = "Animation";
            this.AnimationPage.UseVisualStyleBackColor = true;
            // 
            // RefreshAnimationFileList
            // 
            this.RefreshAnimationFileList.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.RefreshAnimationFileList.Location = new System.Drawing.Point(0, 399);
            this.RefreshAnimationFileList.Margin = new System.Windows.Forms.Padding(1);
            this.RefreshAnimationFileList.Name = "RefreshAnimationFileList";
            this.RefreshAnimationFileList.Size = new System.Drawing.Size(290, 32);
            this.RefreshAnimationFileList.TabIndex = 3;
            this.RefreshAnimationFileList.Text = "새로고침";
            this.RefreshAnimationFileList.UseVisualStyleBackColor = true;
            // 
            // AnimationFileList
            // 
            this.AnimationFileList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.AnimationFileList.FormattingEnabled = true;
            this.AnimationFileList.ItemHeight = 12;
            this.AnimationFileList.Location = new System.Drawing.Point(1, 1);
            this.AnimationFileList.Margin = new System.Windows.Forms.Padding(1);
            this.AnimationFileList.Name = "AnimationFileList";
            this.AnimationFileList.Size = new System.Drawing.Size(291, 376);
            this.AnimationFileList.TabIndex = 2;
            // 
            // PartsPage
            // 
            this.PartsPage.Controls.Add(this.PartsListBox);
            this.PartsPage.Controls.Add(this.RefreshPartsListButton);
            this.PartsPage.Location = new System.Drawing.Point(4, 22);
            this.PartsPage.Name = "PartsPage";
            this.PartsPage.Size = new System.Drawing.Size(290, 431);
            this.PartsPage.TabIndex = 3;
            this.PartsPage.Text = "Parts";
            this.PartsPage.UseVisualStyleBackColor = true;
            // 
            // PartsListBox
            // 
            this.PartsListBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.PartsListBox.FormattingEnabled = true;
            this.PartsListBox.ItemHeight = 12;
            this.PartsListBox.Location = new System.Drawing.Point(1, 1);
            this.PartsListBox.Margin = new System.Windows.Forms.Padding(1);
            this.PartsListBox.Name = "PartsListBox";
            this.PartsListBox.Size = new System.Drawing.Size(291, 376);
            this.PartsListBox.TabIndex = 5;
            // 
            // RefreshPartsListButton
            // 
            this.RefreshPartsListButton.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.RefreshPartsListButton.Location = new System.Drawing.Point(0, 399);
            this.RefreshPartsListButton.Margin = new System.Windows.Forms.Padding(1);
            this.RefreshPartsListButton.Name = "RefreshPartsListButton";
            this.RefreshPartsListButton.Size = new System.Drawing.Size(290, 32);
            this.RefreshPartsListButton.TabIndex = 4;
            this.RefreshPartsListButton.Text = "새로고침";
            this.RefreshPartsListButton.UseVisualStyleBackColor = true;
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(40, 40);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FrameStrip});
            this.statusStrip1.Location = new System.Drawing.Point(0, 668);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Padding = new System.Windows.Forms.Padding(0, 0, 5, 0);
            this.statusStrip1.Size = new System.Drawing.Size(835, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // FrameStrip
            // 
            this.FrameStrip.Name = "FrameStrip";
            this.FrameStrip.Size = new System.Drawing.Size(38, 17);
            this.FrameStrip.Text = "FPS : ";
            // 
            // RenderBox
            // 
            this.RenderBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.RenderBox.Location = new System.Drawing.Point(0, 24);
            this.RenderBox.Margin = new System.Windows.Forms.Padding(1);
            this.RenderBox.Name = "RenderBox";
            this.RenderBox.Size = new System.Drawing.Size(835, 666);
            this.RenderBox.TabIndex = 0;
            this.RenderBox.TabStop = false;
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(40, 40);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileMenu});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(2, 1, 0, 1);
            this.menuStrip1.Size = new System.Drawing.Size(835, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // FileMenu
            // 
            this.FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileFbxConvertMenu,
            this.toolStripSeparator1,
            this.FileOpenMenu,
            this.FileSaveMenu,
            this.toolStripMenuItem1,
            this.FileExitMenu});
            this.FileMenu.Name = "FileMenu";
            this.FileMenu.Size = new System.Drawing.Size(57, 22);
            this.FileMenu.Text = "파일(&F)";
            // 
            // FileFbxConvertMenu
            // 
            this.FileFbxConvertMenu.Name = "FileFbxConvertMenu";
            this.FileFbxConvertMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.FileFbxConvertMenu.Size = new System.Drawing.Size(179, 22);
            this.FileFbxConvertMenu.Text = "Fbx 변환(&C)";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(176, 6);
            // 
            // FileOpenMenu
            // 
            this.FileOpenMenu.Name = "FileOpenMenu";
            this.FileOpenMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.FileOpenMenu.Size = new System.Drawing.Size(179, 22);
            this.FileOpenMenu.Text = "열기(&O)";
            // 
            // FileSaveMenu
            // 
            this.FileSaveMenu.Name = "FileSaveMenu";
            this.FileSaveMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.FileSaveMenu.Size = new System.Drawing.Size(179, 22);
            this.FileSaveMenu.Text = "저장(&S)";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(176, 6);
            // 
            // FileExitMenu
            // 
            this.FileExitMenu.Name = "FileExitMenu";
            this.FileExitMenu.Size = new System.Drawing.Size(179, 22);
            this.FileExitMenu.Text = "종료(E&xit)";
            // 
            // FrameTimer
            // 
            this.FrameTimer.Enabled = true;
            this.FrameTimer.Interval = 1000;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(835, 690);
            this.Controls.Add(this.MainPanel);
            this.ImeMode = System.Windows.Forms.ImeMode.Disable;
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(1);
            this.Name = "MainForm";
            this.Text = "Model Editor";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.MainPanel.ResumeLayout(false);
            this.MainPanel.PerformLayout();
            this.AnimationBarPanel.ResumeLayout(false);
            this.AnimationBarPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.AnimationTrackBar)).EndInit();
            this.SkeletonPanel.ResumeLayout(false);
            this.BottomPanel.ResumeLayout(false);
            this.RightPanel.ResumeLayout(false);
            this.RightSplitContainer.Panel1.ResumeLayout(false);
            this.RightSplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.RightSplitContainer)).EndInit();
            this.RightSplitContainer.ResumeLayout(false);
            this.InspectorTab.ResumeLayout(false);
            this.MaterialPage.ResumeLayout(false);
            this.ModelPage.ResumeLayout(false);
            this.ModelPage.PerformLayout();
            this.ModelPartAdderPanel.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.AniPage.ResumeLayout(false);
            this.AniPage.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.ProjectTab.ResumeLayout(false);
            this.TexturePage.ResumeLayout(false);
            this.ShaderPage.ResumeLayout(false);
            this.AnimationPage.ResumeLayout(false);
            this.PartsPage.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RenderBox)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Panel MainPanel;
        private System.Windows.Forms.PictureBox RenderBox;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem FileMenu;
        private System.Windows.Forms.ToolStripMenuItem FileOpenMenu;
        private System.Windows.Forms.ToolStripMenuItem FileSaveMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem FileExitMenu;
        private System.Windows.Forms.ToolStripMenuItem FileFbxConvertMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel FrameStrip;
        private System.Windows.Forms.Panel RightPanel;
        private System.Windows.Forms.SplitContainer RightSplitContainer;
        private System.Windows.Forms.TabControl InspectorTab;
        private System.Windows.Forms.TabPage ModelPage;
        private System.Windows.Forms.TabPage AniPage;
        private System.Windows.Forms.Splitter RightSplitter;
        private System.Windows.Forms.TabControl ProjectTab;
        private System.Windows.Forms.TabPage TexturePage;
        private System.Windows.Forms.TabPage ShaderPage;
        private System.Windows.Forms.Button RefreshShaderFileList;
        private System.Windows.Forms.ListBox ShaderFileList;
        private System.Windows.Forms.Panel BottomPanel;
        private System.Windows.Forms.ListView LogListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Timer FrameTimer;
        private System.Windows.Forms.TabPage MaterialPage;
        private System.Windows.Forms.TableLayoutPanel MaterialTable;
        private System.Windows.Forms.ListBox TextureFileList;
        private System.Windows.Forms.Button RefreshTextureFileList;
        private System.Windows.Forms.Panel AnimationBarPanel;
        private System.Windows.Forms.TrackBar AnimationTrackBar;
        private System.Windows.Forms.Panel SkeletonPanel;
        private System.Windows.Forms.TreeView SkeletonTreeView;
        private System.Windows.Forms.TabPage AnimationPage;
        private System.Windows.Forms.Button RefreshAnimationFileList;
        private System.Windows.Forms.ListBox AnimationFileList;
        private System.Windows.Forms.TableLayoutPanel ModelLayoutPanel;
        private System.Windows.Forms.Panel ModelPartAdderPanel;
        private System.Windows.Forms.TextBox ModelAdderPathBox;
        private System.Windows.Forms.TabPage PartsPage;
        private System.Windows.Forms.ListBox PartsListBox;
        private System.Windows.Forms.Button RefreshPartsListButton;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TextBox ModelAdderSkeletonBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TextBox AnimationNameTextBox;
        private System.Windows.Forms.TextBox AnimationFilePathTextBox;
        private System.Windows.Forms.Button AnimationAdderButton;
        private System.Windows.Forms.TableLayoutPanel AnimationLayoutPanel;
    }
}


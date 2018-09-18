using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Viewer
{
    class Connect
    {
        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_Create(int handle, float width, float height);

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_Destroy();

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_Update();

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_Render();

        [DllImport("Direct3D.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Cs_WndProc(PeekMsg message);

        [StructLayout(LayoutKind.Sequential)]
        private struct PeekMsg
        {
            public IntPtr hwnd;
            public Message msg;
            public IntPtr wParam;
            public IntPtr lParam;
            public uint time;
            public Point pt;
        }

        [System.Security.SuppressUnmanagedCodeSecurity]
        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        private static extern bool PeekMessage
        (
            out PeekMsg msg
            , IntPtr hwnd
            , uint messageFilterMin
            , uint messageFilterMax
            , uint flag
        );

        private static PictureBox _renderBox;
        public static PictureBox RenderBox
        {
            set { _renderBox = value; }
        }

        public static void Create(object sender, EventArgs e)
        {
            float width = (float)_renderBox.Size.Width;
            float height = (float)_renderBox.Size.Height;

            Cs_Create(_renderBox.Handle.ToInt32(), width, height);
        }

        public static void Destory(object sender, EventArgs e)
        {
            Cs_Destroy();
        }

        public static void Idle(object sender, EventArgs e)
        {
            PeekMsg msg;

            while (true)
            {
                bool bCheck = PeekMessage(out msg, IntPtr.Zero, 0, 0, 0);
                Cs_WndProc(msg);

                if (bCheck == true)
                    break;

                Cs_Update();
                Cs_Render();
            }
        }
    }
}

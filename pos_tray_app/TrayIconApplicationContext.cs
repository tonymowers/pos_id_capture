using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace CH.Alika.POS.TrayApp
{
    // https://www.simple-talk.com/dotnet/.net-framework/creating-tray-applications-in-.net-a-practical-guide/
    class TrayIconApplicationContext : ApplicationContext
    {
        private static readonly string _DefaultTooltip = "Alika Point-Of-Sale";
        private static readonly string _IconFileName = "Logo64x64.ico";
        private System.ComponentModel.Container components;
        private NotifyIcon notifyIcon;
        public TrayIconApplicationContext()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            notifyIcon = new NotifyIcon(components)
            {
                ContextMenuStrip = new ContextMenuStrip(),
                Icon = new Icon(_IconFileName),
                Text = _DefaultTooltip,
                Visible = true
            };
            // notifyIcon.ContextMenuStrip.Opening += ContextMenuStrip_Opening;
            // notifyIcon.DoubleClick += notifyIcon_DoubleClick;
        }

        
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Threading;

namespace CH.Alika.POS.TrayApp
{
    // https://www.simple-talk.com/dotnet/.net-framework/creating-tray-applications-in-.net-a-practical-guide/
    class TrayIconApplicationContext : ApplicationContext
    {
        private static readonly string _DefaultTooltip = "Alika Point-Of-Sale";
        private System.ComponentModel.Container components;
        private NotifyIcon notifyIcon;
        private RemoteWindowService remoteService;
        private SynchronizationContext _uiThreadContext;

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
                Icon = Properties.Resources.TrayIcon64x64,
                Text = _DefaultTooltip,
                Visible = true
            };

            _uiThreadContext = new WindowsFormsSynchronizationContext();
            notifyIcon.ContextMenuStrip.Opening += ContextMenuStrip_Opening;
            notifyIcon.Click += NotifyIcon_Click;
            // notifyIcon.DoubleClick += notifyIcon_DoubleClick;
            remoteService = new RemoteWindowService();
            remoteService.OnScanEvent += HandleScanEvent;
            remoteService.OnScanDeliveredEvent += HandleScanDeliveredEvent;
            remoteService.Activate();
        }

        private void ContextMenuStrip_Opening(object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = false;
            notifyIcon.ContextMenuStrip.Items.Clear();
            notifyIcon.ContextMenuStrip.Items.Add("Menu 1",null,null);
            notifyIcon.ContextMenuStrip.Items.Add("Menu 2", null, null);
            notifyIcon.ContextMenuStrip.Items.Add("Very Long Menu 3", null, null);
        }

        private void NotifyIcon_Click(object sender, EventArgs e)
        {
            
        }

        private void HandleScanEvent(object source, EventArgs e)
        {
            _uiThreadContext.Post((SendOrPostCallback)delegate
            {
                notifyIcon.BalloonTipText = "A document was successfully scanned.";
                notifyIcon.BalloonTipTitle = "Document Scanned";
                notifyIcon.ShowBalloonTip(3);
            }, null);
        }

        private void HandleScanDeliveredEvent(object source, EventArgs e)
        {
            _uiThreadContext.Post((SendOrPostCallback)delegate
            {
                System.Media.SystemSounds.Asterisk.Play();
                notifyIcon.BalloonTipText = "A scanned document was successfully delivered to cloud service";
                notifyIcon.BalloonTipTitle = "Document Scan Delivered";
                notifyIcon.ShowBalloonTip(3);
            }, null);
        }
    }
}

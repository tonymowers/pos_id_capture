using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Threading;
using CH.Alika.POS.TrayApp.Logging;

namespace CH.Alika.POS.TrayApp
{
    // https://www.simple-talk.com/dotnet/.net-framework/creating-tray-applications-in-.net-a-practical-guide/
    class TrayIconApplicationContext : ApplicationContext
    {
        private static readonly string _DefaultTooltip = "Alika Point-Of-Sale";
        private System.ComponentModel.Container components;
        private NotifyIcon notifyIcon;
        private SubscriptionProxy subscriptionProxy;
        private SynchronizationContext _uiThreadContext;

        public TrayIconApplicationContext()
        {
            InitializeComponent();
            MainForm = new MainForm();
            MainForm.WindowState = FormWindowState.Minimized;
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

            notifyIcon.Disposed += (s, ev) =>
            {
                if (System.Windows.Forms.Application.MessageLoop)
                {
                    // WinForms app
                    System.Windows.Forms.Application.Exit();
                }
                else
                {
                    // Console app
                    System.Environment.Exit(1);
                }
            };

            _uiThreadContext = new WindowsFormsSynchronizationContext();
            notifyIcon.ContextMenuStrip.Opening += ContextMenuStrip_Opening;
            notifyIcon.Click += NotifyIcon_Click;
            // notifyIcon.DoubleClick += notifyIcon_DoubleClick;
            subscriptionProxy = new SubscriptionProxy();
            subscriptionProxy.OnScanEvent += HandleScanEvent;
            subscriptionProxy.OnScanDeliveredEvent += HandleScanDeliveredEvent;
            subscriptionProxy.Activate();
        }

        private void ContextMenuStrip_Opening(object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = false;
            notifyIcon.ContextMenuStrip.Items.Clear();
            notifyIcon.ContextMenuStrip.Items.Add("Quit", null, NotifyIcon_Quit);
        }

        private void NotifyIcon_Quit(object sender, EventArgs e)
        {
            // http://stackoverflow.com/questions/9826197/handle-wm-close-message-send-to-c-sharp-tray-app
            notifyIcon.Dispose();
            
        }

        private void NotifyIcon_Click(object sender, EventArgs e)
        {
            var mouseEvent = e as MouseEventArgs;
            if ((mouseEvent != null) && (mouseEvent.Button == MouseButtons.Left))
            {
                _uiThreadContext.Post((SendOrPostCallback)delegate
                {
                    notifyIcon.BalloonTipText = "This application generates notifications about document scanning events";
                    notifyIcon.BalloonTipTitle = "Alika Analytics Point-Of-Sale Application";
                    notifyIcon.BalloonTipIcon = ToolTipIcon.Info;
                    notifyIcon.ShowBalloonTip(3);
                }, null);
            }
        }

        private void HandleScanEvent(object source, ScanEvent e)
        {
            _uiThreadContext.Post((SendOrPostCallback)delegate
            {
                notifyIcon.BalloonTipText = "A document was successfully scanned.";
                notifyIcon.BalloonTipTitle = "Document Scanned";
                notifyIcon.BalloonTipIcon = ToolTipIcon.Info;
                notifyIcon.ShowBalloonTip(3);
            }, null);
        }

        private void HandleScanDeliveredEvent(object source, ScanDeliveryEvent e)
        {
            _uiThreadContext.Post((SendOrPostCallback)delegate
            {
                if (e.ScanDeliveryResult.WasDelivered)
                {
                    try
                    {
                        var balloonTip = Newtonsoft.Json.JsonConvert.DeserializeObject<BalloonTip>(e.ScanDeliveryResult.DeliveryResponse);
                        notifyIcon.BalloonTipTitle = balloonTip.Title;
                        notifyIcon.BalloonTipText = balloonTip.Text;
                        notifyIcon.BalloonTipIcon = balloonTip.Icon;
                    }
                    catch
                    {
                        var balloonTip = new BalloonTip();
                        notifyIcon.BalloonTipText = balloonTip.Text;
                        notifyIcon.BalloonTipTitle = balloonTip.Title;
                        notifyIcon.BalloonTipIcon = balloonTip.Icon;
                    }
                }
                else
                {
                    notifyIcon.BalloonTipText = "Error: " + e.ScanDeliveryResult.DeliveryResponse;
                    notifyIcon.BalloonTipTitle = "Document Scan Delivery Failed";
                    notifyIcon.BalloonTipIcon = ToolTipIcon.Error;
                }
                if (ToolTipIcon.Error.Equals(notifyIcon.BalloonTipIcon) || ToolTipIcon.Warning.Equals(notifyIcon.BalloonTipIcon))
                {
                    System.Media.SystemSounds.Beep.Play();
                }
                else
                {
                    System.Media.SystemSounds.Asterisk.Play(); 
                }
                notifyIcon.ShowBalloonTip(3);
            }, null);
        }
    }
}

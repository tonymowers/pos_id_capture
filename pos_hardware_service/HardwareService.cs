using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using CH.Alika.POS.Hardware;

namespace CH.Alika.POS.Service
{
    //
    // For walkthrough of how to build a Windows Service
    // See: https://msdn.microsoft.com/en-us/library/zt39148a(v=vs.110).aspx
    //
    // For how to log to the Windows EventLog
    // See: https://msdn.microsoft.com/en-us/library/f6567h1s(v=vs.110).aspx
    //
    // For how to build an installer for the service
    // See: https://support.microsoft.com/en-us/help/816169/how-to-create-a-setup-project-for-a-windows-service-application-in-visual-c
    //
    // Alternative installer solutions:
    // http://wixtoolset.org/
    //
    // To see the event log
    // Type in start menu: Management
    // run Computer Management
    //
    // To manage services
    // Type in start menu: services.msc
    //
    public partial class HardwareService : ServiceBase
    {
        private static String _configFileName = AppDomain.CurrentDomain.BaseDirectory + "AlikaPosConfig.txt";
        private MMMDocumentScanner scanner = null;
        private IScanSink scanSink = null;

        public HardwareService()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            base.OnStart(args);
            try
            {
                scanner = new MMMDocumentScanner();
                scanSink = new ScanSinkWebService(_configFileName);
                scanSink.OnScanSinkEvent += HandleScanSinkEvent;
                scanner.OnCodeLineScanEvent += scanSink.HandleCodeLineScan;
                scanner.Activate();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry(this.ServiceName, e.StackTrace,
                                       System.Diagnostics.EventLogEntryType.Warning);
                OnStop();
                throw e;
            }
        }

        private void HandleScanSinkEvent(object sender, ScanSinkEvent e)
        {
            Console.WriteLine(e);
            if (e.IsException)
            {
                EventLog.WriteEntry(this.ServiceName, e.Exception.Message,
                                       System.Diagnostics.EventLogEntryType.Warning, 101);
            }
            else
            {
                EventLog.WriteEntry(this.ServiceName, "Scan delivered successfully",
                                       System.Diagnostics.EventLogEntryType.Information, 100);
            }
        }

        protected override void OnStop()
        {
            cleanup(scanner);
            scanner = null;
            cleanup(scanSink);
            scanSink = null;
        }

        private void cleanup(IDisposable resource)
        {
            if (resource != null)
            {
                try
                {
                    resource.Dispose();
                }
                catch
                {

                }
            }

        }
    }
}

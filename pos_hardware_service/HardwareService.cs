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
    public partial class HardwareService : ServiceBase
    {
        private MMMDocumentScanner scanner = null;
        private IScanSink docSink = null;

        public HardwareService()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            try
            {
                scanner = new MMMDocumentScanner();
                docSink = new ScanSinkWebService();
                scanner.OnCodeLineScanEvent += docSink.HandleCodeLineScan;
                scanner.Activate();
            }
            catch (Exception e)
            {
                OnStop();
                throw e;
            }
        }

        protected override void OnStop()
        {
           cleanup(scanner);
           scanner = null;
           cleanup(docSink);
           docSink = null;
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

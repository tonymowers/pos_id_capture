using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using CH.Alika.POS.Hardware;
using System.ServiceModel;
using CH.Alika.POS.Remote;
using System.ServiceModel.Description;

namespace CH.Alika.POS.Service
{
    //
    // For Services
    // https://msdn.microsoft.com/en-us/library/ms734712.aspx
    // https://www.codeproject.com/Articles/86007/ways-to-do-WCF-instance-management-Per-call-Per

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
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single)] 
    public partial class HardwareService : ServiceBase, IScanner
  
    {
        private static String _configFileName = AppDomain.CurrentDomain.BaseDirectory + "AlikaPosConfig.txt";
        private MMMDocumentScanner scanner = null;
        private IScanSink scanSink = null;
        private ServiceHost serviceHost = null;
        private ISubscriber subscriber = null;

        public HardwareService()
        {
            InitializeComponent();
        }
        public void Subscribe()
        {
            subscriber = OperationContext.Current.GetCallbackChannel<ISubscriber>();
            EventLog.WriteEntry(this.ServiceName, "subscribe called");
        }

        public void Unsubscribe()
        {
            subscriber = null;
            EventLog.WriteEntry(this.ServiceName, "unsubscribe called");
        }

        protected override void OnStart(string[] args)
        {
            base.OnStart(args);
            try
            {
                scanner = new MMMDocumentScanner();
                scanSink = new ScanSinkWebService(_configFileName);
                serviceHost = RemoteFactory.CreateServiceHost(this);
                BindSourceToSink(scanner,scanSink);

                serviceHost.Open();
                scanner.Activate();
            }
            catch (Exception e)
            {
                EventLog.WriteEntry(this.ServiceName, e.Message,
                                       System.Diagnostics.EventLogEntryType.Warning);
                EventLog.WriteEntry(this.ServiceName, e.StackTrace,
                                       System.Diagnostics.EventLogEntryType.Warning);
                OnStop();
                throw e;
            }
        }

        private ServiceHost CreateRemoteServiceHost()
        {
            String pipeLocation = "net.pipe://localhost/AlikaPosService/Scanner";
            ServiceHost selfHost = new ServiceHost(this);
            selfHost.AddServiceEndpoint(typeof(IScanner), new NetNamedPipeBinding(NetNamedPipeSecurityMode.None), pipeLocation);
            ServiceMetadataBehavior smb = new ServiceMetadataBehavior();
            smb.MetadataExporter.PolicyVersion = PolicyVersion.Policy15;
            selfHost.Description.Behaviors.Add(smb);

            // Add MEX endpoint
            selfHost.AddServiceEndpoint(
                ServiceMetadataBehavior.MexContractName,
                MetadataExchangeBindings.CreateMexNamedPipeBinding(),
                pipeLocation + "/mex"
                );
            return selfHost;
        }

        private void BindSourceToSink(IScanSource scanSource, IScanSink scanSink)
        {
            scanSink.OnScanSinkEvent += HandleScanSinkEvent;
            scanner.OnCodeLineScanEvent += HandleCodeLineScan;
        }

        private void HandleCodeLineScan(object sender, CodeLineScanEvent e)
        {
            if (subscriber != null)
            {
                subscriber.OnScanEvent();
            }
            scanSink.HandleCodeLineScan(sender, e);
        }

        private void HandleScanSinkEvent(object sender, ScanSinkEvent e)
        {
            if (e.IsException)
            {
                EventLog.WriteEntry(this.ServiceName, e.Exception.Message,
                                       System.Diagnostics.EventLogEntryType.Warning, 101);
                return;
            }
            if (subscriber != null)
            {
                subscriber.OnScanDeliveredEvent();
            }
        }

        protected override void OnStop()
        {
            if (serviceHost != null)
            {
                serviceHost.Close();
                serviceHost = null;
            }
                
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

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
using CH.Alika.POS.Service.Logging;
using System.Threading.Tasks;

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
        private static readonly ILog log = LogProvider.For<HardwareService>();
        private static readonly String _configFileName = AppDomain.CurrentDomain.BaseDirectory + "AlikaPosConfig.txt";
        private MMMSwipeReader scanner = null;
        private IScanStore scanStoreCloud = null;
        private ServiceHost serviceHost = null;
        private SubscriberGroup subscribers = null;

        public HardwareService()
        {
            InitializeComponent();
        }

        private SubscriberAsync GetSubscriberAsync()
        {
            return new SubscriberAsync(OperationContext.Current.GetCallbackChannel<ISubscriber>());
        }

        public void Subscribe()
        {
            using (LogProvider.OpenNestedContext("Subscriber_Subscribe"))
            {
                log.Info("Remote client subscribed");
                subscribers.Add(GetSubscriberAsync()); 
            }
        }

        public void Unsubscribe()
        {
            using (LogProvider.OpenNestedContext("Subscriber_Unsubscribe"))
            {
                log.Info("Remote client unsubscribed");
                subscribers.Remove(GetSubscriberAsync());
            }
        }

        protected override void OnStart(string[] args)
        {
            base.OnStart(args);
            log.Info("Service starting");
            try
            {
                subscribers = new SubscriberGroup();
                scanner = new MMMSwipeReader();
                scanStoreCloud = new ScanStoreCloud(_configFileName);
                serviceHost = RemoteFactory.CreateServiceHost(this);
                BindScanSourceToScanStore(scanner, scanStoreCloud);

                serviceHost.Open();
                scanner.Activate();
                log.InfoFormat("Service started and listening at ",RemoteFactory.PipeLocation);
            }
            catch (Exception e)
            {
                log.ErrorFormat("Service exception while starting [{0}]", e);
                EventLog.WriteEntry(this.ServiceName, e.Message,
                                       System.Diagnostics.EventLogEntryType.Warning);
                EventLog.WriteEntry(this.ServiceName, e.StackTrace,
                                       System.Diagnostics.EventLogEntryType.Warning);
                OnStop();
                throw e;
            }
        }

        private void BindScanSourceToScanStore(IScanSource scanSource, IScanStore scanSink)
        {
            scanSink.OnScanStoreEvent += HandleScanStoreEvent;
            scanner.OnCodeLineScanEvent += HandleCodeLineScan;
        }

        private void HandleCodeLineScan(object sender, CodeLineScanEvent e)
        {
            log.Info("Handling CodeLineScan");
            log.DebugFormat("Begin processing Scan", e);
            subscribers.NotifyAll(e);

            try
            {
                log.Info("Putting scanned document asynchronously into cloud");
                scanStoreCloud.CodeLineDataPutAsync(e);
            }
            catch (Exception ex)
            {
                log.ErrorFormat("Exception during delivery of scan [{0}]", ex);
            }

            log.Debug("End processing Scan");
        }

        private void HandleScanStoreEvent(object sender, ScanStoreEvent e)
        {
            log.Info("Handle result of delivery of scan");
            log.DebugFormat("Begin handling Scan delivery result [{0}]", e);
            if (e.IsException)
            {
                log.InfoFormat("Create Windows Event Log Entry of [{0}]", e);
                EventLog.WriteEntry(this.ServiceName, e.Exception.Message,
                                       System.Diagnostics.EventLogEntryType.Warning, 101);

            }
            subscribers.NotifyAll(e);
            log.Debug("End handling Scan delivery result ");
        }

        protected override void OnStop()
        {
            log.Info("Service stopping");
            if (serviceHost != null)
            {
                serviceHost.Close();
                serviceHost = null;
            }

            cleanup(scanner);
            scanner = null;
            cleanup(scanStoreCloud);
            scanStoreCloud = null;
            cleanup(subscribers);
            subscribers = null;
            log.Info("Service stopped");
        }

        private void cleanup(IDisposable resource)
        {
            log.DebugFormat("Disposing of resource [{0}]", resource);
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

        internal void StartFromConsole(string[] args)
        {
            OnStart(args);
        }

        internal void StopFromConsole()
        {
            OnStop();
        }
    }
}

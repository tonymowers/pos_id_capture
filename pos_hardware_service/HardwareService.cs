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
        private IScanStore scanSink = null;
        private ServiceHost serviceHost = null;
        private SubscriberAsync subscriber = null;

        public HardwareService()
        {
            InitializeComponent();
        }
        public void Subscribe()
        {
            log.Info("Remote client subscribed");
            subscriber = GetSubscriberAsync(); ;
            subscriber.Closed += new EventHandler(Subscriber_Closed);
            EventLog.WriteEntry(this.ServiceName, "subscribe called");
        }

        private void Subscriber_Closed(object sender, EventArgs e)
        {
            log.Info("Subscriber closed");
            if (sender.Equals(subscriber)) {
                log.Debug("Removed subscriber from list of subscribers");
                subscriber = null;
            }
        }

        private SubscriberAsync GetSubscriberAsync()
        {
            return new SubscriberAsync(OperationContext.Current.GetCallbackChannel<ISubscriber>());
        }
        public void Unsubscribe()
        {
            log.Info("Remote client unsubscribed");
            Subscriber_Closed(GetSubscriberAsync(),EventArgs.Empty);
            EventLog.WriteEntry(this.ServiceName, "unsubscribe called");
        }

        protected override void OnStart(string[] args)
        {
            base.OnStart(args);
            log.Info("Service starting");
            try
            {
                scanner = new MMMSwipeReader();
                scanSink = new ScanStoreCloud(_configFileName);
                serviceHost = RemoteFactory.CreateServiceHost(this);
                BindSourceToSink(scanner, scanSink);

                serviceHost.Open();
                scanner.Activate();
                log.Info("Service started");
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

        private void BindSourceToSink(IScanSource scanSource, IScanStore scanSink)
        {
            scanSink.OnScanStoreEvent += HandleScanStoreEvent;
            scanner.OnCodeLineScanEvent += HandleCodeLineScan;
        }

        private void HandleCodeLineScan(object sender, CodeLineScanEvent e)
        {
            log.Info("Handling CodeLineScan");
            log.DebugFormat("Begin processing Scan", e);

            if (subscriber != null)
            {
                try
                {
                    log.Info("Notifying subscriber asynchronously of scanned document");
                    subscriber.NotifySubscriberAsync(e);
                }
                catch (Exception ex)
                {
                    log.ErrorFormat("Failed to deliver Scan Event to subscriber [{0}]", e);
                    log.ErrorFormat("Exception during handling of Scan Event notification [{0}]", ex);
                }
            }
            try
            {
                log.Info("Putting scanned document asynchronously into cloud");
                scanSink.CodeLineDataPutAsync(e);
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
            else
            {
                if (subscriber != null)
                {

                    try
                    {
                        log.InfoFormat("Notify subscribers asynchronously of scan delivery result [{0}]");
                        subscriber.NotifySubscriberAsync(e);
                    }
                    catch (Exception ex)
                    {
                        log.ErrorFormat("Failed to complete delivery notification [{0}]", e);
                        log.ErrorFormat("Exception during delivery notification [{0}]", ex);
                    }
                }
            }
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
            cleanup(scanSink);
            scanSink = null;
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

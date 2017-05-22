using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RestSharp;
using CH.Alika.POS.Hardware.Logging;
using System.Threading.Tasks;

namespace CH.Alika.POS.Hardware
{
    public class ScanStoreCloud : IScanStore
    {
        private static readonly ILog log = LogProvider.For<ScanStoreCloud>();
        private string _configFileName;

        public event EventHandler<ScanStoreEvent> OnScanStoreEvent;
        public ScanStoreCloud(String configFileName)
        {
            log.InfoFormat("ScanCloudStore configured using file [{0}]", configFileName);
            _configFileName = configFileName;
        }

        public Task<ScanStoreEvent> CodeLineDataPutAsync(CodeLineScanEvent e)
        {
            log.DebugFormat("Put CodeLineData into store async [{0}]", e);
            Task<ScanStoreEvent> task = Task.Factory.StartNew<ScanStoreEvent>(() =>
            {
                ScanStoreEvent scanStoreEvent;
                using (LogProvider.OpenNestedContext("Task_CodeLineDataPut"))
                {
                    log.Info("Putting scan in cloud");
                    try
                    {
                        ScanStoreRestImpl service = new ScanStoreRestImpl(_configFileName,e);
                        log.Debug("Begin call CodeLineDataPut web service");
                        String response = service.CodeLineDataPut(e);
                        log.Debug("End call CodeLineDataPut web service");
                        scanStoreEvent = new ScanStoreEvent(response);
                    }
                    catch (Exception ex)
                    {
                        log.ErrorFormat("Exception while putting scan into cloud [{0}]", ex.Message);
                        scanStoreEvent = new ScanStoreEvent(ex);
                    }
                    NotifyListeners(scanStoreEvent);
                }
                return scanStoreEvent;
            });
            return task;
        }

        private void NotifyListeners(ScanStoreEvent scanStoreEvent)
        {
            if (scanStoreEvent.IsException)
            {
                log.ErrorFormat("Notifying listeners of failure puttting scan in cloud [{0}]", scanStoreEvent.Exception.Message);
            }
            else
            {
                log.Info("Notifying listeners of success putting scan in cloud");
            }
            log.DebugFormat("Begin notify listeners of ScanStoreEvent [{0}]", scanStoreEvent);
            try { OnScanStoreEvent(this, scanStoreEvent); }
            catch { }
            log.Debug("End notify listeners of ScanStoreEvent");

        }

        public void Dispose()
        {
            log.Debug("ScanStoreCloud disposed");
        }
    }
}

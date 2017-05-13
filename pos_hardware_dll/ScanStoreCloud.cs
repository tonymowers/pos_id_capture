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
            log.InfoFormat("Construct using file [{0}]", configFileName);
            _configFileName = configFileName;
        }

        public Task<ScanStoreEvent> CodeLineDataPutAsync(CodeLineScanEvent e)
        {
            log.DebugFormat("Put CodeLineData into store async [{0}]", e);
            Task<ScanStoreEvent> task = Task.Factory.StartNew<ScanStoreEvent>(() =>
            {
                ScanStoreEvent scanStoreEvent;
                try
                {
                    ScanStoreRestImpl service = new ScanStoreRestImpl(_configFileName);
                    log.Debug("Call CodeLineDataPut web service");
                    String response = service.CodeLineDataPut(e.CodeLineData);
                    log.Debug("Return CodeLineDataPut web service");
                    scanStoreEvent = new ScanStoreEvent(response);
                }
                catch (Exception ex)
                {
                    log.DebugFormat("Exception while putting scan into cloud [{0}]", e);
                    scanStoreEvent = new ScanStoreEvent(ex);
                }
                NotifyListeners(scanStoreEvent);
                return scanStoreEvent;
            });
            return task;
        }

        private void NotifyListeners(ScanStoreEvent scanStoreEvent)
        {
            log.DebugFormat("Begin notify listeners of ScanStoreEvent [{0}]", scanStoreEvent);
            try
            {
                OnScanStoreEvent(this, scanStoreEvent);
            }
            catch { }
            log.DebugFormat("End notify listeners of ScanStoreEvent [{0}]", scanStoreEvent);
        }

        public void Dispose()
        {
            log.Info("Dispose");
        }
    }
}

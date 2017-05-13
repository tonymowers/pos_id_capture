using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RestSharp;
using CH.Alika.POS.Hardware.Logging;

namespace CH.Alika.POS.Hardware
{
    public class ScanStoreCloudProxy : IScanStore
    {
        private static readonly ILog log = LogProvider.For<ScanStoreCloudProxy>();
        private string _configFileName;

        public event EventHandler<ScanStoreEvent> OnScanStoreEvent;
        public ScanStoreCloudProxy(String configFileName)
        {
            log.InfoFormat("Construct using file [{0}]", configFileName);
            _configFileName = configFileName;
        }

        public void CodeLineDataPut(object sender, CodeLineScanEvent e)
        {
            log.DebugFormat("Begin putting CodeLineData into Store [{0}]", e);
            ScanStoreEvent scanStoreEvent;
            try
            {
                ScanStoreRestImpl service = new ScanStoreRestImpl(_configFileName);
                String response = service.CodeLineDataPut(e.CodeLineData);
                scanStoreEvent = new ScanStoreEvent(response);
            }
            catch (Exception ex)
            {
                log.DebugFormat("Exception while putting scan into cloud [{0}]", e);
                scanStoreEvent = new ScanStoreEvent(ex);
            }
            NotifyListeners(scanStoreEvent);
            log.Debug("End putting CodeLineData into Store");
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

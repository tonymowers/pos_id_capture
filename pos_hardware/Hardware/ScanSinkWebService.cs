using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RestSharp;

namespace CH.Alika.POS.Hardware
{
    public class ScanSinkWebService : IScanSink
    {
        public event EventHandler<ScanSinkEvent> OnScanSinkEvent;
        public void HandleCodeLineScan(object sender, CodeLineScanEvent e)
        {
            try
            {
                ScanStoreApi service = new ScanStoreApi();
                String response = service.CodeLineDataPut(e.CodeLineData);
                OnScanSinkEvent(this, new ScanSinkEvent(response));
            }
            catch (Exception ex)
            {
                OnScanSinkEvent(this, new ScanSinkEvent(ex));
            }
        }

        public void Dispose()
        {

        }
    }
}

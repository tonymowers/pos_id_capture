using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public interface IScanSink : IDisposable
    {
        event EventHandler<ScanSinkEvent> OnScanSinkEvent;
        void HandleCodeLineScan(object sender, CodeLineScanEvent e);
    }
}

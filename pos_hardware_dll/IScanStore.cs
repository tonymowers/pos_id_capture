using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public interface IScanStore : IDisposable
    {
        event EventHandler<ScanStoreEvent> OnScanStoreEvent;
        void CodeLineDataPut(object sender, CodeLineScanEvent e);
    }
}

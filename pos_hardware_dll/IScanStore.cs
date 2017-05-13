using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CH.Alika.POS.Hardware
{
    public interface IScanStore : IDisposable
    {
        event EventHandler<ScanStoreEvent> OnScanStoreEvent;
        Task<ScanStoreEvent> CodeLineDataPutAsync(CodeLineScanEvent e);
    }
}

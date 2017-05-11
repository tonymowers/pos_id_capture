using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    class ScanSinkConsole : IScanSink
    {
        public event EventHandler<ScanSinkEvent> OnScanSinkEvent;
        public void HandleCodeLineScan(object sender, CodeLineScanEvent e)
        {
            Console.WriteLine(Newtonsoft.Json.JsonConvert.SerializeObject(e));
        }

        public void Dispose()
        {
            
        }
    }
}

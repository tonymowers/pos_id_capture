using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Remote;

namespace CH.Alika.POS.TrayApp
{
    class ScanEvent :  EventArgs
    {
        public ScanResult ScanResult { get; set; }
    }
}

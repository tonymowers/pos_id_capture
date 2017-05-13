using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public interface IScanSource : IDisposable
    {
        event EventHandler<CodeLineScanEvent> OnCodeLineScanEvent;
        void Activate();
    }
}

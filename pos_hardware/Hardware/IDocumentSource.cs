using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public interface IDocumentSource : IDisposable
    {
        string DocumentSourceId { get; }
        event EventHandler<CodeLineScanEvent> OnCodeLineScanEvent;
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    class ConsoleDocumentSink : IDocumentSink
    {
        public void HandleCodeLineScan(object sender, CodeLineScanEvent e)
        {
            IDocumentSource source = sender as IDocumentSource;
            Console.WriteLine(source.DocumentSourceId);
            Console.WriteLine(Newtonsoft.Json.JsonConvert.SerializeObject(e));
        }

        public void Dispose()
        {
            
        }
    }
}

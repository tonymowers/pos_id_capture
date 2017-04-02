using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RestSharp;

namespace CH.Alika.POS.Hardware
{
    public class ScanSinkWebService : IScanSink
    {
        public void HandleCodeLineScan(object sender, CodeLineScanEvent e)
        {
            try
            {
                TalkTalkApi service = new TalkTalkApi();
                service.CodeLineDataPut(e.CodeLineData);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
        }

        public void Dispose()
        {

        }
    }
}

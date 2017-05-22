using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Hardware;
using CH.Alika.POS.ConsoleApp.Logging;

namespace CH.Alika.POS.ConsoleApp
{
    class ScannerLocallyLocated : IScanner, IDisposable
    {
        private static readonly ILog log = LogProvider.For<ScannerLocallyLocated>();
        private static String _configFileName = AppDomain.CurrentDomain.BaseDirectory + "AlikaPosConfig.txt";
        private MMMSwipeReader scanner;
        private IScanStore documentSink;

        public void Activate()
        {
            log.Info("Activating connection to local 3M Scanner and remote web service for delivery");
            scanner = new MMMSwipeReader();
            documentSink = new ScanStoreCloud(_configFileName);
            try
            {
                documentSink.OnScanStoreEvent += HandleScanSinkEvent;
                scanner.OnScanSourceEvent += HandleScanSourceEvent;
                scanner.OnCodeLineScanEvent += HandleCodeLineScanEvent;
                scanner.Activate();
            }
            catch (PosHardwareException e)
            {
                Console.WriteLine(e.Message);
            }
        }

        public void Dispose()
        {
            if (documentSink != null)
            {
                documentSink.Dispose();
                documentSink = null;
            };

            if (scanner != null)
            {
                scanner.Dispose();
                scanner = null;
            }
        }

        public override String ToString()
        {
            return String.Format("ScannerLocallyLocated [{0}]", scanner == null ? "" : scanner.ToString());
        }

        private void HandleCodeLineScanEvent(object sender, CodeLineScanEvent e)
        {
            documentSink.CodeLineDataPutAsync(e);
        }

        static void HandleScanSourceEvent(object sender, ScanSourceEvent e)
        {
            if (log.IsDebugEnabled())
                Console.WriteLine(e);
        }

        static void HandleScanSinkEvent(object sender, ScanStoreEvent e)
        {
            Console.WriteLine(e);
            
        }
    }
}

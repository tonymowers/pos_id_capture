using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Hardware;

namespace CH.Alika.POS.Hardware
{
    class Program
    {
        private static String _configFileName = AppDomain.CurrentDomain.BaseDirectory + "AlikaPosConfig.txt";
        static void Main(string[] args)
        {
            Console.WriteLine("Press enter key to exit");
            Console.WriteLine();
            using (MMMSwipeReader scanner = new MMMSwipeReader())
            using (IScanStore documentSink = new ScanStoreCloud(_configFileName))
            {
                try
                {
                    scanner.OnCodeLineScanEvent += delegate(object sender, CodeLineScanEvent e) { documentSink.CodeLineDataPutAsync(e); };
                    scanner.Activate();
                    Console.WriteLine(scanner);
                }
                catch (PosHardwareException e)
                {
                    Console.WriteLine(e.Message);
                }
                Console.ReadLine();
            }
        }

    }
}

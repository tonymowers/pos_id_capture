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
            using (MMMCR100SwipeReader scanner = new MMMCR100SwipeReader())
            using (IScanStore documentSink = new ScanStoreCloudProxy(_configFileName))
            {
                try
                {
                    scanner.OnCodeLineScanEvent += documentSink.CodeLineDataPut;
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

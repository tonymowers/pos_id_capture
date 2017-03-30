using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Hardware;

namespace CH.Alika.POS
{
    class Program
    {
       
        static void Main(string[] args)
        {
            Console.WriteLine("Press enter key to exit");
            Console.WriteLine();
            using (MMMDocumentScanner scanner = new MMMDocumentScanner())
            using (IDocumentSink documentSink = new ConsoleDocumentSink())
            {
                try
                {
                    scanner.OnCodeLineScanEvent += documentSink.HandleCodeLineScan;
                    scanner.Initialize();
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

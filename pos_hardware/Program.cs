using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Press enter key to exit");
            Console.WriteLine();
            using (MMMDocumentScanner scanner = new MMMDocumentScanner())
            {
                try
                {
                    scanner.OnCodeLineDataEvent += HandleNewSwipeData;
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

        static void HandleNewSwipeData(object sender, CodeLineDataEvent e)
        {
            Console.WriteLine(Newtonsoft.Json.JsonConvert.SerializeObject(e.CodeLineData));
        }
    }
}

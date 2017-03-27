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
                scanner.OnCustomerDataRead += HandleNewCustomerData;
                scanner.Initialize();
                Console.ReadLine();
            }
        }

        static void HandleNewCustomerData(object sender, EventArgs e)
        {
            Console.WriteLine("new customer data");
        }
    }
}

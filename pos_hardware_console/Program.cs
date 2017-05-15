using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Hardware;
using CH.Alika.POS.Remote;
using System.ServiceModel;
using System.Media;
using CH.Alika.POS.ConsoleApp.Logging;
[assembly: log4net.Config.XmlConfigurator(ConfigFileExtension = "log4net", Watch = true)]
namespace CH.Alika.POS.ConsoleApp
{




    class Program
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(Program));

        static void Main(string[] args)
        {
            using (LogProvider.OpenNestedContext("AlikaPosConsole_MAIN"))
            {
                log.Info("Starting AlikaPosConsole Application");

                Console.WriteLine("Press enter key to exit");
                Console.WriteLine();

                using (IScanner scanner = CreateScanner(args.Length == 0))
                {
                    try
                    {
                        log.InfoFormat("Activate hardware event subscriber [{0}]", scanner);
                        scanner.Activate();
                        Console.WriteLine(scanner);
                        Console.WriteLine("Scan documents now");
                        Console.ReadLine();
                    }
                    catch (Exception e)
                    {
                        log.FatalFormat("Failure to activate subscriber [{0}] exception [{1}]", scanner, e);
                    }
                }

                log.Info("Terminating AlikaPosConsole Application");
            }
        }

        static IScanner CreateScanner(bool useRemotelyLocatedScanner)
        {
            if (useRemotelyLocatedScanner)
                return new ScannerRemotelyLocated();
            else
                return new ScannerLocallyLocated();
        }
    }
}

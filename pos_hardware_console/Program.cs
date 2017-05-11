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
    interface IScanner : IDisposable
    {
        void Activate();
    }

    class WindowsServiceProxy : IScanner, ISubscriber, IDisposable
    {
        private static readonly ILog log = LogProvider.For<WindowsServiceProxy>();
        CH.Alika.POS.Remote.IScanner client;
        DuplexChannelFactory<CH.Alika.POS.Remote.IScanner> clientFactory;

        public void Activate()
        {
            log.Info("Activating connection to remote windows service based scanner and web service");
            clientFactory = RemoteFactory.CreateClientFactory(this);
            client = clientFactory.CreateChannel();
            client.Subscribe();
        }

        public void OnScanEvent()
        {
            log.Info("Scan event detected");
            Console.WriteLine("document scanned");
        }

        public void OnScanDeliveredEvent()
        {
            log.Info("Document delivery detected");
            Console.WriteLine("document delivered");
            SystemSounds.Exclamation.Play();
        }

        public void Dispose()
        {
            log.Info("Disposing of service proxy");
            if (client != null)
            {
                client.Unsubscribe();
                client = null;
            };

            if (clientFactory != null)
            {
                clientFactory.Close();
                clientFactory = null;
            }
        }

        public override String ToString()
        {
            return String.Format("Windows Service Proxy[{0}]", RemoteFactory.PipeLocation);
        }
    }

    class LocalScanner : IScanner, IDisposable
    {
        private static readonly ILog log = LogProvider.For<LocalScanner>();
        private static String _configFileName = AppDomain.CurrentDomain.BaseDirectory + "AlikaPosConfig.txt";
        private MMMDocumentScanner scanner;
        private IScanSink documentSink;

        public void Activate()
        {
            log.Info("Activating connection to local 3M Scanner and remote web service for delivery");
            scanner = new MMMDocumentScanner();
            documentSink = new ScanSinkWebService(_configFileName);
            try
            {
                documentSink.OnScanSinkEvent += HandleScanSinkEvent;
                scanner.OnCodeLineScanEvent += documentSink.HandleCodeLineScan;
                scanner.Activate();
                Console.WriteLine(scanner);
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
            return scanner.ToString();
        }

        static void HandleScanSinkEvent(object sender, ScanSinkEvent e)
        {
            Console.WriteLine(e);
        }
    }

    class Program
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(Program));

        static void Main(string[] args)
        {
            log.Info("Starting AlikaPosConsole Application");

            Console.WriteLine("Press enter key to exit");
            Console.WriteLine();

            using (IScanner scanner = CreateScanner(args.Length == 0))
            {
                try
                {
                    scanner.Activate();
                    Console.WriteLine(scanner);
                    Console.ReadLine();
                }
                catch (Exception e)
                {
                    log.Error(scanner);
                    log.Error("Failure while tring to connect to scanner", e);

                }
            }

            log.Info("Terminating AlikaPosConsole Application");
        }

        static IScanner CreateScanner(bool useServiceProxy)
        {
            if (useServiceProxy)
                return new WindowsServiceProxy();
            else
                return new LocalScanner();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Hardware;
using CH.Alika.POS.Remote;
using System.ServiceModel;
using System.Media;
// using CH.Alika.POS.ServiceReference1;

namespace CH.Alika.POS
{
    interface IScanner : IDisposable
    {
        void Activate();
    }

    class WindowsServiceProxy : IScanner, ISubscriber, IDisposable
    {
        CH.Alika.POS.Remote.IScanner client;
        DuplexChannelFactory<CH.Alika.POS.Remote.IScanner> clientFactory;

        public void Activate()
        {
            clientFactory = RemoteFactory.CreateClientFactory(this);
            client = clientFactory.CreateChannel();
            client.Subscribe();
        }

        public void OnScanEvent()
        {
            Console.WriteLine("document scanned");
        }

        public void OnScanDeliveredEvent()
        {
            Console.WriteLine("document delivered");
            SystemSounds.Exclamation.Play();
        }

        public void Dispose()
        {
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
        private static String _configFileName = AppDomain.CurrentDomain.BaseDirectory + "AlikaPosConfig.txt";
        private MMMDocumentScanner scanner;
        private IScanSink documentSink;

        public void Activate()
        {
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

        static void Main(string[] args)
        {

            Console.WriteLine("Press enter key to exit");
            Console.WriteLine();

            using (IScanner scanner = CreateScanner(args.Length == 0))
            {
                scanner.Activate();
                Console.WriteLine(scanner);
                Console.ReadLine();
            }


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

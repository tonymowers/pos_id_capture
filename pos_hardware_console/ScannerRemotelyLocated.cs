using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Media;
using CH.Alika.POS.Remote;
using CH.Alika.POS.ConsoleApp.Logging;
using System.ServiceModel;

namespace CH.Alika.POS.ConsoleApp
{

    class ScannerRemotelyLocated : IScanner, ISubscriber, IDisposable
    {
        private static readonly ILog log = LogProvider.For<ScannerRemotelyLocated>();
        CH.Alika.POS.Remote.IScanner client;
        DuplexChannelFactory<CH.Alika.POS.Remote.IScanner> clientFactory;

        public void Activate()
        {
            log.InfoFormat("Activating hardware subscription [{0}]", RemoteFactory.PipeLocation);
            clientFactory = RemoteFactory.CreateClientFactory(this);
            client = clientFactory.CreateChannel();
            client.Subscribe();
        }

        public void HandlerScan(ScanResult r)
        {
            log.InfoFormat("Scan event detected, validationResult [{0}] contents [{1}]",r.ValidationResult,r.Contents);
            Console.WriteLine(String.Format( "document scanned validationResult [{0}] contents [{1}]",r.ValidationResult,r.Contents));
        }

        public void HandleScanDelivered(ScanDeliveryResult r)
        {
            log.InfoFormat("Document delivery result, wasDelivered [{0}] deliveryResponse [{1}]", r.WasDelivered, r.DeliveryResponse);
            Console.WriteLine(String.Format("document delivery result, wasDelivered [{0}] deliveryResponse [{1}]", r.WasDelivered, r.DeliveryResponse));
            SystemSounds.Exclamation.Play();
        }

        public void Dispose()
        {
            log.Info("Disposing of service proxy");
            if (client != null)
            {
                try
                {
                    client.Unsubscribe();
                }
                catch (Exception) { }
                finally
                {
                    client = null;
                }
            };

            if (clientFactory != null)
            {
                try
                {
                    clientFactory.Close();
                }
                catch (Exception) { }
                finally
                {
                    clientFactory = null;
                }
            }
        }

        public override String ToString()
        {
            return String.Format("ScannerRemotelyLocated [{0}]", RemoteFactory.PipeLocation);
        }
    }
}

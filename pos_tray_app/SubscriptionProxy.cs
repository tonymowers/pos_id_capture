using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Remote;
using System.ServiceModel;
using CH.Alika.POS.TrayApp.Logging;
using System.Timers;

namespace CH.Alika.POS.TrayApp
{
    class SubscriptionProxy : IDisposable
    {
        private static readonly ILog log = LogProvider.For<SubscriptionProxy>();
        private static readonly int SUBSCRIPTION_RETRY_INTERVAL = 5000;
        public event EventHandler<ScanEvent> OnScanEvent;
        public event EventHandler<ScanDeliveryEvent> OnScanDeliveredEvent;

        private IScanner client;
        private DuplexChannelFactory<CH.Alika.POS.Remote.IScanner> clientFactory;
        private bool _isUnfaulted = true;

        public void Activate()
        {
            log.Info("Activating client side subscription");
            Subscribe();
        }

        private void Subscription_Closed(object sender, EventArgs e)
        {
            log.Debug("Subscription channel closed");
        }

        private void Subscription_Faulted(object sender, EventArgs e)
        {
            log.Debug("Subscription channel faulted");
            if (_isUnfaulted)
            {
                _isUnfaulted = false;
                log.Info("Subscription channel has become faulted");
            }
            RetrySubscribe(SUBSCRIPTION_RETRY_INTERVAL);
        }

        private void Subscribe()
        {
            try
            {
                clientFactory = RemoteFactory.CreateClientFactory(new Subscriber(this));
                client = clientFactory.CreateChannel();
                var communicationObject = client as ICommunicationObject;
                if (communicationObject != null)
                {
                    log.Debug("register channel state change listeners");
                    communicationObject.Closed += Subscription_Closed;
                    communicationObject.Faulted += Subscription_Faulted;
                }
                client.Subscribe();
                _isUnfaulted = true;
                log.InfoFormat("Successfully subscribed to [{0}]", RemoteFactory.PipeLocation);
            }
            catch (EndpointNotFoundException)
            {
                log.Debug("Service is not ready to receive subscriptions");
                // Subscription_Faulted callback will be triggered
            }
        }

        private void RetrySubscribe(int retryInterval)
        {
            log.Debug("Retry to subscribe");
            if ((clientFactory != null) && (clientFactory.State == CommunicationState.Faulted))
            {
                clientFactory.Abort();
                clientFactory = null;
                client = null;
            }
            Timer timer = new Timer(retryInterval);
            timer.AutoReset = false;
            timer.Elapsed += (object sender, ElapsedEventArgs e) =>
            {
                Subscribe();
            };
            timer.Start();
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

        private class Subscriber : ISubscriber
        {
            SubscriptionProxy _parent;
            public Subscriber(SubscriptionProxy parent)
            {
                _parent = parent;
            }

            public void HandlerScan(ScanResult r)
            {
                if (_parent.OnScanEvent != null)
                    _parent.OnScanEvent(_parent, new ScanEvent { ScanResult = r });
            }

            public void HandleScanDelivered(ScanDeliveryResult r)
            {
                if (_parent.OnScanDeliveredEvent != null)
                    _parent.OnScanDeliveredEvent(_parent, new ScanDeliveryEvent { ScanDeliveryResult = r });
            }
        }
    }
}

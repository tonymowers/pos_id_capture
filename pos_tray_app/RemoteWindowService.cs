using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Remote;
using System.ServiceModel;

namespace CH.Alika.POS.TrayApp
{
    class RemoteWindowService : IDisposable
    {
        public event EventHandler<EventArgs> OnScanEvent;
        public event EventHandler<EventArgs> OnScanDeliveredEvent;

        private IScanner client;
        private DuplexChannelFactory<CH.Alika.POS.Remote.IScanner> clientFactory;

        public void Activate()
        {
            clientFactory = RemoteFactory.CreateClientFactory(new Subscriber(this));
            client = clientFactory.CreateChannel();
            client.Subscribe();
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
            RemoteWindowService _parent;
            public Subscriber(RemoteWindowService parent)
            {
                _parent = parent;
            }

            public void HandlerScan(int validationResult, string contents)
            {
                if (_parent.OnScanEvent != null)
                    _parent.OnScanEvent(_parent, new EventArgs());
            }

            public void HandleScanDelivered(bool wasDelivered, string deliveryResponse)
            {
                if (_parent.OnScanDeliveredEvent != null)
                    _parent.OnScanDeliveredEvent(_parent, new EventArgs());
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CH.Alika.POS.Hardware;
using System.Collections.Concurrent;
using CH.Alika.POS.Service.Logging;

namespace CH.Alika.POS.Service
{
    class SubscriberGroup : IDisposable
    {
        private static readonly ILog log = LogProvider.For<SubscriberGroup>();
        private ConcurrentDictionary<SubscriberAsync, SubscriberAsync> _subscribers = new ConcurrentDictionary<SubscriberAsync, SubscriberAsync>();

        private void Subscriber_Closed(object sender, EventArgs e)
        {
            using (LogProvider.OpenNestedContext("Subscriber_Closed"))
            {
                log.Debug("Subscriber closed");
                var subscriber = sender as SubscriberAsync;
                if (subscriber != null)
                {
                    log.Debug("Remove subscriber from list of subscribers");
                    Remove(subscriber);
                }
            }
        }

        public void Add(SubscriberAsync subscriber)
        {
            log.Debug("Add");
            if (_subscribers.TryAdd(subscriber, subscriber))
            {
                log.Info("Subscriber joined group");
                subscriber.Closed += Subscriber_Closed;
            }
        }

        public void Remove(SubscriberAsync subscriber)
        {
            log.Debug("Add");
            SubscriberAsync dummy = null;
            if (_subscribers.TryRemove(subscriber, out dummy))
            {
                log.Info("Subscriber left group");
                subscriber.Closed -= Subscriber_Closed;
            }
        }

        public void NotifyAll(ScanStoreEvent e)
        {
            log.Info("NotifyAll subscribers asynchronously of scan delivery result");
            foreach (var subscriber in _subscribers.Values)
            {       
                try
                {
                    log.Info("Notify subscriber async of scan delivery result");
                    subscriber.NotifySubscriberAsync(e);
                }
                catch (Exception ex)
                {
                    log.ErrorFormat("Failed to complete delivery notification [{0}]", e);
                    log.ErrorFormat("Exception during delivery notification [{0}]", ex);
                }
            }
        }

        public void NotifyAll(CodeLineScanEvent e)
        {
            log.Info("NotifyAll subscribers asynchronously of scanned document");
            foreach (var subscriber in _subscribers.Values)
            {  
                try
                {
                    log.Info("Notifying subscriber async of scanned document");
                    subscriber.NotifySubscriberAsync(e);
                }
                catch (Exception ex)
                {
                    log.ErrorFormat("Failed to deliver Scan Event to subscriber [{0}]", e);
                    log.ErrorFormat("Exception during handling of Scan Event notification [{0}]", ex);
                }
            }
        }

        public void Dispose()
        {
            log.Debug("Disposing of SubscriberGroup");
            foreach (var subscriber in _subscribers.Values)
            {
                log.Debug("Removing close listener from subscriber"); 
                subscriber.Closed -= new EventHandler(Subscriber_Closed);
            }
            _subscribers.Clear();
        }
    }
}

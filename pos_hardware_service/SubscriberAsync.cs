using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Remote;
using System.Threading.Tasks;
using CH.Alika.POS.Hardware;
using CH.Alika.POS.Service.Logging;
using System.ServiceModel;

namespace CH.Alika.POS.Service
{
    class SubscriberAsync
    {
        private static readonly ILog log = LogProvider.For<SubscriberAsync>();

        public bool IsOpen
        {
            get
            {
                return _isOpen;
            }
            private set
            {
                if ((value == false) && (value != _isOpen))
                {
                    log.Debug("Notify listeners SubscriberAsync isClosed");
                    try { Closed(this, EventArgs.Empty); }
                    catch { };
                    Closed = null;
                }
                _isOpen = value;
            }
        }

        public event EventHandler Closed;

        private bool _isOpen = false;
        private ISubscriber _subscriber;
        public SubscriberAsync(ISubscriber subscriber)
        {
            IsOpen = true;
            _subscriber = subscriber;
            
            var CommunicationObject = subscriber as ICommunicationObject;
            if (CommunicationObject != null)
            {
                CommunicationObject.Faulted += new EventHandler(CommunicationObject_Faulted);
                CommunicationObject.Closed += new EventHandler(CommunicationObject_Closed);
            }
        }

        private void CommunicationObject_Faulted(object sender, EventArgs e)
        {
            log.DebugFormat("Communcation channel faulted [{0}]", e);
            IsOpen = false;
        }

        private void CommunicationObject_Closed(object sender, EventArgs e)
        {
            log.DebugFormat("Communcation channel closed [{0}]", e);
            IsOpen = false;
        }

        public Task NotifySubscriberAsync(CodeLineScanEvent e)
        {
            return Task.Factory.StartNew(() =>
            {
                using (LogProvider.OpenNestedContext("Task_NotifySubscriber_CodeLineScan"))
                {
                    log.Info("Notify remote subscriber of scan");
                    log.DebugFormat("Begin call remote notification of scan [{0}]", e);
                    try { _subscriber.HandlerScan((int)(e.CodeLineData.CodelineValidationResult), e.CodeLineData.Surname); }
                    catch (Exception ex)
                    {
                        log.WarnFormat("Unable to notify remote subscriber of scan  [{0}]", ex.Message);
                        IsOpen = false;
                    }
                    log.DebugFormat("End call remote notification of scan");
                }
            });
        }

        public Task NotifySubscriberAsync(ScanStoreEvent e)
        {
            return Task.Factory.StartNew(() =>
            {
                using (LogProvider.OpenNestedContext("Task_NofitySubscriber_ScanDelivery"))
                {
                    log.Info("Notify remote subscriber of scan delivery result");
                    log.DebugFormat("Begin call remote notification of scan delivery result [{0}]", e);
                    try { _subscriber.HandleScanDelivered(!e.IsException, e.DeliveryResponse); }
                    catch (Exception ex)
                    {
                        log.WarnFormat("Unable to notify remote subscriber of scan delivery result [{0}]", ex.Message);
                        IsOpen = false;
                    }
                    log.DebugFormat("End call remote subscriber scan delivery result [{0}]", e);
                }
            });
        }

        public override bool Equals(object obj)
        {
            var that = obj as SubscriberAsync;

            if (that == null)
            {
                return false;
            }

            return this._subscriber.Equals(that._subscriber);
        }

        public override int GetHashCode()
        {
            return this._subscriber.GetHashCode();
        }
    }
}

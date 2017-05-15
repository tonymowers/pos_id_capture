using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CH.Alika.POS.Remote;
using System.Threading.Tasks;
using CH.Alika.POS.Hardware;
using CH.Alika.POS.Service.Logging;

namespace CH.Alika.POS.Service
{
    class SubscriberAsync
    {
        private static readonly ILog log = LogProvider.For<SubscriberAsync>();

        private ISubscriber _subscriber;
        public SubscriberAsync(ISubscriber subscriber)
        {
            _subscriber = subscriber;
        }

        public Task NotifySubscriberAsync(CodeLineScanEvent e)
        {
            return Task.Factory.StartNew(() =>
            {
                using (LogProvider.OpenNestedContext("Task_NotifySubscriber_CodeLineScan"))
                {
                    log.Info("Notify remote subscriber of scan");
                    log.DebugFormat("Begin call remote notification of scan [{0}]", e);
                    try { _subscriber.HandlerScan((int)(e.CodeLineData.CodelineValidationResult),e.CodeLineData.Surname); }
                    catch (Exception ex)
                    {
                        log.WarnFormat("Unable to notify remote subscriber of scan  [{0}]", ex.Message);
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
                    try { _subscriber.HandleScanDelivered(!e.IsException,e.DeliveryResponse); }
                    catch (Exception ex)
                    {
                        log.WarnFormat("Unable to notify remote subscriber of scan delivery result [{0}]", ex.Message);
                    }
                    log.DebugFormat("End call remote subscriber scan delivery result [{0}]", e);
                }
            });
        }
    }
}

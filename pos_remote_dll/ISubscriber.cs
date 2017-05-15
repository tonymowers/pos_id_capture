using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;

namespace CH.Alika.POS.Remote
{
    public interface ISubscriber
    {
        [OperationContract(IsOneWay = true)]
        void HandlerScan(int validationResult, string contents);

        [OperationContract(IsOneWay = true)]
        void HandleScanDelivered(bool wasDelivered, string deliveryResponse);
    }
}

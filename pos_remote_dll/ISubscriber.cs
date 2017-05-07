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
        void OnScanEvent();

        [OperationContract(IsOneWay = true)]
        void OnScanDeliveredEvent();
    }
}

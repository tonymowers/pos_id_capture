using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using System.Runtime.Serialization;

namespace CH.Alika.POS.Remote
{
    public interface ISubscriber
    {
        [OperationContract(IsOneWay = true)]
        void HandlerScan(ScanResult result);

        [OperationContract(IsOneWay = true)]
        void HandleScanDelivered(ScanDeliveryResult result);
    }

    [DataContract]
    public class ScanResult
    {
        [DataMember]
        public int ValidationResult { get; set; }

        [DataMember]
        public string Contents { get; set; }
    }

    [DataContract]
    public class ScanDeliveryResult
    {
        [DataMember]
        public bool WasDelivered { get; set; }

        [DataMember]
        public string DeliveryResponse { get; set; }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public class ScanStoreEvent : EventArgs
    {
        private Exception _exception;
        public String DeliveryResponse { get; private set; }

        public ScanStoreEvent(String deliveryResponse)
        {
            DeliveryResponse = deliveryResponse;
        }

        public Boolean IsException
        {
            get { return _exception != null; }
        }
        public Exception Exception
        {
            get { return _exception; }
        }
        public ScanStoreEvent(Exception ex)
        {
            this._exception = ex;
        }

        public override string ToString()
        {
            return String.Format("ScanStoreEvent [{0}]", IsException ? _exception.Message : "scan delivered (" + DeliveryResponse + ")");
        }
    }
}

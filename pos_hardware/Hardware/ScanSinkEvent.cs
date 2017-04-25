using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public class ScanSinkEvent : EventArgs
    {
        private Exception _exception;
        private String response;

        public ScanSinkEvent(String response)
        {
            this.response = response;
        }

        public Boolean IsException
        {
            get { return _exception != null; }
        }
        public Exception Exception
        {
            get { return _exception;  }
        }
        public ScanSinkEvent(Exception ex)
        {
            this._exception = ex;
        }

        public override string ToString()
        {
            return "ScanSinkEvent: " + (IsException ? _exception.Message: "scan delivered (" + response + ")");
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public class ScanSinkEvent : EventArgs
    {
        private Exception ex;
        private String response;

        public ScanSinkEvent(String response)
        {
            this.response = response;
        }

        public Boolean IsException()
        {
            return ex != null;
        }

        public ScanSinkEvent(Exception ex)
        {
            // TODO: Complete member initialization
            this.ex = ex;
        }

        public override string ToString()
        {
            return "ScanSinkEvent: " + (IsException() ? ex.Message: "scan delivered (" + response + ")");
        }
    }
}

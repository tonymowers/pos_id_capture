using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public class PosHardwareException : Exception
    {
        public PosHardwareException()
        {
        }
 
        public PosHardwareException(string message)
            : base(message)
        {
        }

        public PosHardwareException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }
}

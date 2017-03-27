using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public class CodeLineDataEvent : EventArgs
    {
        public MMM.Readers.CodelineData CodeLineData { get; private set; }

        public CodeLineDataEvent(MMM.Readers.CodelineData codeLineData)
        {
            CodeLineData = codeLineData;
        }
    }
}

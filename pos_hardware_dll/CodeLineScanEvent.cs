using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public class CodeLineScanEvent : EventArgs
    {
        public MMM.Readers.CodelineData CodeLineData { get; private set; }
        public bool IsInvalid
        {
            get
            {
                return CodeLineData.CodelineValidationResult == MMM.Readers.CheckDigitResult.CDR_Invalid;
            }
        }
        public CodeLineScanEvent(MMM.Readers.CodelineData codeLineData)
        {
            CodeLineData = codeLineData;
        }
    }
}

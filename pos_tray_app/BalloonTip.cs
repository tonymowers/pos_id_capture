using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CH.Alika.POS.TrayApp
{
    public class BalloonTip
    {
        public String Title { get; set; }
        public String Text { get; set; }
        public String Severity { get; set; }
        public ToolTipIcon Icon
        {
            get
            {
                var severity = Severity.ToLower();
                if ("warn".Equals(severity)) 
                    return ToolTipIcon.Warning;
                
                if ("error".Equals(severity))
                    return ToolTipIcon.Error;
                
                if ("info".Equals(severity))
                    return ToolTipIcon.Info;

                if ("none".Equals(severity))
                    return ToolTipIcon.None;

                return ToolTipIcon.None;
            }
        }

        public BalloonTip()
        {
            Title = "Document Scan Delivered";
            Text = "A scanned document was delivered to cloud";
            Severity = "info";
        }
    }
}

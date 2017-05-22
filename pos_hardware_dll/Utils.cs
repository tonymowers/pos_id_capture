using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace CH.Alika.POS.Hardware
{
    public class Utils
    {
        public static string RetrieveLongUrlFromGoogle(string shortUrl)
        {
            string longUrl = shortUrl;
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(shortUrl);
            req.Timeout = 10000; // 10 seconds
            req.KeepAlive = false;
            req.Method = "HEAD";
            req.AllowAutoRedirect = false;

            HttpWebResponse myResp = (HttpWebResponse)req.GetResponse();
            if (myResp.StatusCode == HttpStatusCode.MovedPermanently)
            {
                longUrl = myResp.GetResponseHeader("Location");
            }

            return longUrl;
        }

        public static bool IsConfigurationEvent(CodeLineScanEvent e)
        {
            MMM.Readers.CodelineData data = e.CodeLineData;
            bool result = (data.CodelineValidationResult == MMM.Readers.CheckDigitResult.CDR_Valid) 
                && data.Line1.StartsWith("PZXXX");

            return result;
        }

        public static MrzBasedConfigurationData ConfigurationData(CodeLineScanEvent e)
        {
            MMM.Readers.CodelineData data = e.CodeLineData;
            return new MrzBasedConfigurationData(data.Line1, data.Line2);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public class MrzBasedConfigurationData
    {
        public String ShortURL { get; private set; }
        public String ClientId { get; private set; }
        public String AccessKey { get; private set; }
        public String ProtocolVersion { get; private set; }

        public MrzBasedConfigurationData(String line1, String line2)
        {
            ShortURL = decodeGoogleShortUrl(line1.Substring(5).Replace("<",""));
            ClientId = line2.Substring(28, 14).Replace("<","");
            AccessKey = line2.Substring(0, 9).Replace("<","");
            ProtocolVersion = line2.Substring(13, 2).Replace("<", "");
        }

        private string decodeGoogleShortUrl(String str)
        {
            String result = "";
            int i = 0;
            while (i < str.Length)
            {
                try
                {
                    int code = (str[i] - 'A') * 16 + (str[i + 1] - 'A');
                    result += Convert.ToChar(code);
                }
                catch { }
                i = i + 2;
            }

            return String.Format("https://goo.gl/{0}",result);
        }

        public override string ToString()
        {
            return String.Format("ProgramData ShortUrl = [{0}], Username = [{1}], PIN = [{2}]", ShortURL, ClientId, AccessKey);
        }

        public string RetrieveLongUrl()
        {
            return Utils.RetrieveLongUrlFromGoogle(ShortURL);
        }
    }
}

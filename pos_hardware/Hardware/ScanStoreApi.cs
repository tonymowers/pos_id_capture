using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RestSharp;
using System.IO;

namespace CH.Alika.POS.Hardware
{
    public class ScanStoreApi
    {
        private ScanStoreConfig Settings;
        public ScanStoreApi()
        {
            try
            {
                String dir = AppDomain.CurrentDomain.BaseDirectory;
                string text = System.IO.File.ReadAllText(dir + @"ScanStoreConfig.txt");
                Settings = Newtonsoft.Json.JsonConvert.DeserializeObject<ScanStoreConfig>(text);
            }
            catch (FileNotFoundException e)
            {
                throw new ConfigNotFoundException(e);
            }
        }

        private String Execute<T>(RestRequest request) where T : new()
        {
            // See http://restsharp.org/
            var client = new RestClient();
            client.BaseUrl = new System.Uri(Settings.BaseUrl);
            var response = client.Execute<T>(request);

            if (response.ErrorException != null)
            {
                const string message = "Error retrieving response.  Check inner details for more info.";
                var twilioException = new ApplicationException(message, response.ErrorException);
                throw twilioException;
            }
            return response.Content;
        }

        public String CodeLineDataPut(MMM.Readers.CodelineData codeLineData)
        {
            var request = new RestRequest(Method.POST);
            Dictionary<string,object> parameters = new Dictionary<string,object>();
            request.AddJsonBody(new JsonRpcRequest()
            {
                Method = "ci_put",
                Params = new Dictionary<string, object>()
                {
                   { "sourceID" , "shopA" },
                   { "codeLineData" , codeLineData }
                }
            });
            return Execute<VOID>(request);
        }

        private class VOID
        {
        }

        private class JsonRpcRequest
        {
            public string Method { get; set; }
            public Object Params { get; set; }
        }

    }

    class ScanStoreConfig
    {
        public String BaseUrl { get; set; }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RestSharp;

namespace CH.Alika.POS.Hardware
{
    public class TalkTalkApi
    {
        private TalkTalkApiConfig Settings;
        public TalkTalkApi()
        {
            String dir = AppDomain.CurrentDomain.BaseDirectory;
            string text = System.IO.File.ReadAllText(dir + @"TalkTalkConfig.txt");
            Settings  = Newtonsoft.Json.JsonConvert.DeserializeObject<TalkTalkApiConfig>(text);
        }

        private T Execute<T>(RestRequest request) where T : new()
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
            Console.WriteLine(response.Content);
            return response.Data;
        }

        public void CodeLineDataPut(MMM.Readers.CodelineData codeLineData)
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
            Execute<VOID>(request);
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

    class TalkTalkApiConfig
    {
        public String BaseUrl { get; set; }
    }
}

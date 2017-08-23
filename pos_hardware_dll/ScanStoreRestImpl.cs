using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RestSharp;
using System.IO;

namespace CH.Alika.POS.Hardware
{

    public class ScanStoreRestImpl
    {
        private ScanStoreConfig Settings;
        public ScanStoreRestImpl(String configFileName, CodeLineScanEvent e)
        {
            try
            {
                CreateScanStoreConfigFileIfNecessary(configFileName, e);
                string text = System.IO.File.ReadAllText(configFileName);
                Settings = Newtonsoft.Json.JsonConvert.DeserializeObject<ScanStoreConfig>(text);
            }
            catch (FileNotFoundException ex)
            {
                throw new ConfigNotFoundException(ex);
            }
        }

        private void CreateScanStoreConfigFileIfNecessary(String configFileName, CodeLineScanEvent e)
        {
            if (!System.IO.File.Exists(configFileName) && Utils.IsConfigurationEvent(e))
            {
                var configData = Utils.ConfigurationData(e);
                var longUrl = configData.RetrieveLongUrl();
                ScanStoreConfig settings = new ScanStoreConfig()
                {
                    ClientId = configData.ClientId,
                    AccessKey = configData.AccessKey,
                    BaseUrl = longUrl,
                    ProtocolVersion = configData.ProtocolVersion
                };
                settings.Write(configFileName);
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

        public String CodeLineDataPut(CodeLineScanEvent e)
        {
            if (string.IsNullOrWhiteSpace(Settings.ProtocolVersion)) {
                return CodeLineDataPutV1(e);
            } else {
                return CodeLineDataPutV2(e);
            }
        }

        private String CodeLineDataPutV1(CodeLineScanEvent e)
        {
            var codeLineData = e.CodeLineData;
            var request = new RestRequest(Method.POST);
            Dictionary<string, object> parameters = new Dictionary<string, object>();
            request.AddJsonBody(new JsonRpcRequestV1()
            {
                Method = "ci_put",
                Params = new Dictionary<string, object>()
                {
                   { "clientId" , Settings.ClientId },
                   { "accessKey", Settings.AccessKey },
                   { "codeLineData" , codeLineData }
                }
            });
            return Execute<VOID>(request);
        }

        private String CodeLineDataPutV2(CodeLineScanEvent e)
        {
            var codeLineData = e.CodeLineData;
            var request = new RestRequest(Method.POST);
            Dictionary<string, object> parameters = new Dictionary<string, object>();
            request.AddJsonBody(new Dictionary<string, object>()
            {
                { "clientId" , Settings.ClientId },
                { "accessKey", Settings.AccessKey },
                { "codeLineData" , codeLineData }
            });
            return Execute<VOID>(request);
        }

        private class VOID
        {
        }

        private class JsonRpcRequestV1
        {
            public string Method { get; set; }
            public Object Params { get; set; }
        }


        private class ScanStoreConfig
        {
            public String BaseUrl { get; set; }
            public String ClientId { get; set; }
            public String AccessKey { get; set; }
            public String ProtocolVersion { get; set; }

            public static ScanStoreConfig Read(String fileName)
            {
                string text = System.IO.File.ReadAllText(fileName);
                return Newtonsoft.Json.JsonConvert.DeserializeObject<ScanStoreConfig>(text);
            }

            public void Write(String fileName)
            {
                System.IO.File.WriteAllText(fileName, Newtonsoft.Json.JsonConvert.SerializeObject(this));
            }

            public static bool ScanStoreConfigExists(String fileName)
            {
                bool exists = true;
                try
                {
                    Read(fileName);
                }
                catch
                {
                    exists = false;
                }
                return exists;
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using System.ServiceModel.Description;

namespace CH.Alika.POS.Remote
{
    public class RemoteFactory
    {
        public readonly static String PipeLocation = "net.pipe://localhost/AlikaPosService/Scanner";

        public static DuplexChannelFactory<IScanner> CreateClientFactory(ISubscriber subscriber)
        {
            return new DuplexChannelFactory<IScanner>(
                    subscriber,
                    new NetNamedPipeBinding(NetNamedPipeSecurityMode.None),
                    new EndpointAddress(PipeLocation));
        }

        public static ServiceHost CreateServiceHost(IScanner scanner)
        {
            ServiceHost selfHost = new ServiceHost(scanner);
            selfHost.AddServiceEndpoint(typeof(IScanner), new NetNamedPipeBinding(NetNamedPipeSecurityMode.None), PipeLocation);
            ServiceMetadataBehavior smb = new ServiceMetadataBehavior();
            smb.MetadataExporter.PolicyVersion = PolicyVersion.Policy15;
            selfHost.Description.Behaviors.Add(smb);

            // Add MEX endpoint
            selfHost.AddServiceEndpoint(
                ServiceMetadataBehavior.MexContractName,
                MetadataExchangeBindings.CreateMexNamedPipeBinding(),
                PipeLocation + "/mex"
                );
            return selfHost;
        } 
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using CH.Alika.POS.Service.Logging;
[assembly: log4net.Config.XmlConfigurator(ConfigFileExtension = "log4net", Watch = true)]

namespace CH.Alika.POS.Service
{
    static class Program
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(Program));
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            using (LogProvider.OpenNestedContext("AlikaPosService_Main"))
            {
                if (Environment.UserInteractive)
                {
                    Console.WriteLine("Press ENTER key to quit");
                    HardwareService service = new HardwareService();
                    service.StartFromConsole(args);
                    Console.ReadLine();
                    service.StopFromConsole();
                }
                else
                {
                    ServiceBase[] ServicesToRun;
                    ServicesToRun = new ServiceBase[] 
			    { 
				    new HardwareService() 
			    };
                    ServiceBase.Run(ServicesToRun);
                }
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using CH.Alika.POS.TrayApp.Logging;
[assembly: log4net.Config.XmlConfigurator(ConfigFileExtension = "log4net", Watch = true)]

namespace CH.Alika.POS.TrayApp
{
    static class Program
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(Program));
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            using (LogProvider.OpenNestedContext("Program_Main"))
            {
                log.Info("Start AlikaPosTrayApp");
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                ApplicationContext applicationContext = new TrayIconApplicationContext();
                // applicationContext.MainForm = new Form1();
                Application.Run(applicationContext);
                log.Info("Terminate AlikaPosTrayApp");
            }
        }
    }
}

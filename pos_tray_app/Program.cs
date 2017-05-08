using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace CH.Alika.POS.TrayApp
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            ApplicationContext applicationContext = new TrayIconApplicationContext();
            // applicationContext.MainForm = new Form1();
            Application.Run(applicationContext);
        }
    }
}

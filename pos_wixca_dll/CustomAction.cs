using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Deployment.WindowsInstaller;
using System.Diagnostics;

namespace CH.Alika.POS.WIX
{
    // http://stackoverflow.com/questions/1186575/using-wm-close-in-c-sharp
    // https://www.codeproject.com/Articles/584105/Prompt-user-to-close-applications-on-install-unins
    // https://blogs.msdn.microsoft.com/jschaffe/2012/10/23/creating-wix-custom-actions-in-c-and-passing-parameters/
    public class CustomActions
    {
        [CustomAction]
        public static ActionResult CloseApps(Session session)
        {
            session.Log("Begin CloseApps Action");
            var processNames = session["ProcessNames"].Split(',');
            foreach (var processName in processNames)
            {
                session.Log("Close [" + processName + "]");
                foreach (var process in Process.GetProcessesByName(processName))
                {
                    // process.Kill();
                    process.CloseMainWindow();
                }
            }

            session.Log("End CloseApps Action");
            return ActionResult.Success;
        }
    }
}

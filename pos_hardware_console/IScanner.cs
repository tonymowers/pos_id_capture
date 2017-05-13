using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.ConsoleApp
{
    interface IScanner : IDisposable
    {
        void Activate();
    }
}

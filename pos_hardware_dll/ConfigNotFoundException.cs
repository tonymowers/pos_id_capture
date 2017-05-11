using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CH.Alika.POS.Hardware
{
    class ConfigNotFoundException : Exception
    {
        private FileNotFoundException _fileNotFoundException;

        public String FileName { get { return _fileNotFoundException.FileName; } }

        public ConfigNotFoundException(FileNotFoundException e)
            : base("Configuration file not found: " + e.FileName, e)
        {
            this._fileNotFoundException = e;
        }


    }
}

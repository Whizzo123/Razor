using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Razor
{
    public class Log
    {
        public void Print(string message)
        {
            InternalManager.Print_Message(message);
        }
    }
}

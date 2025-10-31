using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Razor;

namespace Sandbox
{
    public class PlayerMovement : Razor.System
    {
        public override void Run(float deltaTime)
        {
            Log logger = new Log();
            logger.Print("Running print function from internal call");
        }
    }
}

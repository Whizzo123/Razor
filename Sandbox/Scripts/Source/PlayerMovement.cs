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
            List<UInt32> entities = Scene.GetEntitiesWithTransforms();
            Log logger = new Log();
            foreach (var entity in entities)
            {
                logger.Print("Running print function from internal call entity id " + entity);
            }
        }
    }
}

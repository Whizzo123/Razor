using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Razor;

namespace Sandbox
{
    public class TestBoxBodySystem : Razor.System
    {
        public override void Run(float deltaTime)
        {
            var entities = Scene.GetEntitiesWithScriptComponent<TestBox>();
            Log logger = new Log();
            foreach (uint id in entities)
            {
                for(int i = 0; i < Collision.GetEventCount(id); i++) {
                    CollisionEvent colEvent = Collision.GetEvent(id, i);
                    logger.Print("Collided with entity:" + id);
                }
            }
        }
    }
}

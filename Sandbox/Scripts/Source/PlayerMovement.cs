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
            var entities = Scene.GetEntitiesWithScriptComponent<Player>();
            Log logger = new Log();
            foreach (uint id in entities)
            {
                Player player = Scene.GetComponent<Player>(id);
                if (player == null) continue;

                foreach (CollisionEvent colEvent in Collision.GetEvents(id))
                {
                    if(colEvent.Type == CollisionEventType.Started)
                    {
                        logger.Print("Collided with entity: " + colEvent.OtherEntityId);
                        player.Direction *= -1;
                    }
                }

                var pos = Transform.GetPosition(id);
                Transform.SetPosition(id, pos.X + (5.0f * player.Direction * deltaTime), pos.Y, pos.Z);
            }
        }
    }
}

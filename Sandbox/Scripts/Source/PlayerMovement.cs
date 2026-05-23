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
                Player player = Scene.GetScriptComponent<Player>(id);
                if (player == null) continue;
                var pos = Transform.GetPosition(id);

                if (pos.X < -20.0f )
                {
                   IncrementScore(0);
                   Transform.SetPosition(id, 0, 0, pos.Z);
                   return;
                }
                else if (pos.X > 20.0f)
                {
                    IncrementScore(1);
                    Transform.SetPosition(id, 0, 0, pos.Z);
                    return;
                }

                foreach (CollisionEvent colEvent in Collision.GetEvents(id))
                {
                    if(colEvent.Type == CollisionEventType.Started)
                    {
                        player.Direction *= -1;
                    }
                }
                
                Transform.SetPosition(id, pos.X + (5.0f * player.Direction * deltaTime), pos.Y, pos.Z);
            }
        }

        private void IncrementScore(int index)
        {
             // We need to get the score component and increment it 
            var scoreEntities = Scene.GetEntitiesWithScriptComponent<Score>();
            foreach (uint scoreEntity in scoreEntities) 
            {
                Score score = Scene.GetScriptComponent<Score>(scoreEntity);
                if (score.playerIndex == index)
                {
                    score.score += 1;
                }
            }
        }
    }
}

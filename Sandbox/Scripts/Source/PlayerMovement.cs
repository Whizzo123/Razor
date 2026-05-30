using System;
using System.Collections.Generic;
using System.IO.Pipelines;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading.Tasks.Dataflow;
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

                if (pos.X < -15.0f )
                {
                   IncrementScore(0);
                   Transform.SetPosition(id, 0, 0, pos.Z);
                   player.YDirection = 0.0f;
                   player.BaseSpeed = 5.0f;
                   return;
                }
                else if (pos.X > 15.0f)
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
                        player.BaseSpeed += 0.25f;
                        var otherPos = Transform.GetPosition(colEvent.OtherEntityId);
                        if (otherPos.Y - 0.1f > pos.Y)
                        {
                            player.YDirection = -0.5f;
                        }
                        else if (otherPos.Y + 0.1f < pos.Y)
                        {
                            player.YDirection = 0.5f;
                        }
                        else
                        {
                            player.YDirection = 0.0f;
                        }
                        
                    }
                }
                
                if (pos.Y > 11.0f || pos.Y < -11.0f)
                {
                    player.YDirection *= -1;
                }

                Transform.SetPosition(id, pos.X + (player.BaseSpeed * player.Direction * deltaTime), pos.Y + (player.BaseSpeed * player.YDirection * deltaTime), pos.Z);
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

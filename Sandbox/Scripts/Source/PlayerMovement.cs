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
            uint[] entities = Scene.GetEntitiesWithScriptComponent<Player>();
            Log logger = new Log();
            foreach (var entity in entities)
            {
                Player player = Scene.GetComponent<Player>(entity);
                if (player != null)
                {
                    logger.Print("Player Name is: " + player.Name);
                }
                else
                {
                    logger.Print("We are null ahahahaha");
                }
            }
        }
    }
}

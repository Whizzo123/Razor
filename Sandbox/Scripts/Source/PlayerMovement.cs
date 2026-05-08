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
              logger.Print("Player Name is: " + player.Name);
              var pos = Transform.GetPosition(id);
              if (Input.IsKeyPressed(RazorKey.W))
                  Transform.SetPosition(id, pos.X, pos.Y, pos.Z + 5.0f * deltaTime);
              if (Input.IsKeyPressed(RazorKey.S))
                  Transform.SetPosition(id, pos.X, pos.Y, pos.Z - 5.0f * deltaTime);
          }
        }
    }
}

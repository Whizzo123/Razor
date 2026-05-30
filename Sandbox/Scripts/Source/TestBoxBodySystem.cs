using System.Threading.Tasks.Dataflow;
using System;
using Razor;

namespace Sandbox
{
    public class TestBoxBodySystem : Razor.System
    {
        public override void Run(float deltaTime)
        {
            var entities = Scene.GetEntitiesWithScriptComponent<TestBox>();
            Log logger = new Log();
            foreach(uint id in entities)
            {
                var pos = Transform.GetPosition(id);
                TestBox box = Scene.GetScriptComponent<TestBox>(id);
                if(box.PlayerIndex == 0)
                {
                    if (Input.IsKeyPressed(RazorKey.W))
                        Transform.SetPosition(id, pos.X, Math.Clamp(pos.Y + 5.0f * deltaTime, -7.0f, 7.0f), pos.Z);
                    if (Input.IsKeyPressed(RazorKey.S))
                        Transform.SetPosition(id, pos.X, Math.Clamp(pos.Y - 5.0f * deltaTime, -7.0f, 7.0f), pos.Z);
                }
                else
                {
                    if (Input.IsKeyPressed(RazorKey.A))
                        Transform.SetPosition(id, pos.X, Math.Clamp(pos.Y + 5.0f * deltaTime, -7.0f, 7.0f), pos.Z);
                    if (Input.IsKeyPressed(RazorKey.D))
                        Transform.SetPosition(id, pos.X, Math.Clamp(pos.Y - 5.0f * deltaTime, -7.0f, 7.0f), pos.Z);
                }
            }
        }
    }
}

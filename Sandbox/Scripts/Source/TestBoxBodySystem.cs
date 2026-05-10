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
                foreach (CollisionEvent colEvent in Collision.GetEvents(id))
                {
                    logger.Print("Collided with entity: " + colEvent.OtherEntityId);
                }
            }
        }
    }
}

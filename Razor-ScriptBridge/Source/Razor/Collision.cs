using System;

namespace Razor
{
    // Note: Jolt Persisted contacts (ongoing collisions between frames) are not surfaced here.
    // To track "is currently colliding" state, maintain it yourself using Started/Ended events.
    public enum CollisionEventType { Started = 0, Ended = 1 }

    public struct CollisionEvent
    {
        public CollisionEventType Type;
        public uint OtherEntityId;
    }

    public static class Collision
    {
        // OtherEntityId is set to this value when the other body has no matching ECS entity.
        public const uint UnknownEntity = uint.MaxValue;

        public static int GetEventCount(uint entityId)
            => InternalCalls.CollisionGetEventCount(entityId);

        public static CollisionEvent GetEvent(uint entityId, int index)
            => InternalCalls.CollisionGetEvent(entityId, index);

        public static CollisionEvent[] GetEvents(uint entityId)
        {
            int count = InternalCalls.CollisionGetEventCount(entityId);
            if (count == 0) return Array.Empty<CollisionEvent>();
            CollisionEvent[] result = new CollisionEvent[count];
            for (int i = 0; i < count; i++)
                result[i] = InternalCalls.CollisionGetEvent(entityId, i);
            return result;
        }
    }
}

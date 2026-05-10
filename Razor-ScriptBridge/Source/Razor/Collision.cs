namespace Razor
{
    public enum CollisionEventType { Started = 0, Ended = 1 }

    public struct CollisionEvent
    {
        public CollisionEventType Type;
        public uint OtherEntityId;
    }

    public static class Collision
    {
        public static int GetEventCount(uint entityId)
            => InternalCalls.CollisionGetEventCount(entityId);

        public static CollisionEvent GetEvent(uint entityId, int index)
            => InternalCalls.CollisionGetEvent(entityId, index);
    }
}

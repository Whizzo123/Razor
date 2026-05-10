namespace Razor
{
    public static class Transform
    {
        public static (float X, float Y, float Z) GetPosition(uint entityId)
            => InternalCalls.GetTransformPosition(entityId);

        public static void SetPosition(uint entityId, float x, float y, float z)
            => InternalCalls.SetTransformPosition(entityId, x, y, z);

        public static void SetPosition(uint entityId, (float X, float Y, float Z) pos)
            => InternalCalls.SetTransformPosition(entityId, pos.X, pos.Y, pos.Z);
    }
}

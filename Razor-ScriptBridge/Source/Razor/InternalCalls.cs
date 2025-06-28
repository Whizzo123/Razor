using System;
using System.Runtime.CompilerServices;

namespace Razor
{
    public static class InternalCalls
    {
        #region Entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);
        #endregion

        #region Utilities
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Print_Message(string message);
        #endregion
    }
}
using System;
using System.Runtime.CompilerServices;

namespace Razor
{
    internal static unsafe class InternalCalls
    {
        internal static delegate* unmanaged[Cdecl]<bool> Entity_HasComponent;
        internal static delegate* unmanaged[Cdecl]<IntPtr, void> Print_Message;
    }

        public static class InternalManager
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
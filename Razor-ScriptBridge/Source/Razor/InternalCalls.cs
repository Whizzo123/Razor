using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace Razor
{
    internal static unsafe class InternalCalls
    {
        internal static delegate* unmanaged<ulong, Type, bool> Entity_HasComponent;
        internal static delegate* unmanaged[Cdecl]<IntPtr, void> Print_Message;

        public static void LogMsg(string msg)
        {
            IntPtr p = Marshal.StringToHGlobalAnsi(msg);

            try
            {
                Print_Message(p);
            }
            finally
            {
                Marshal.FreeHGlobal(p);
            }
        }

        public static bool HasComponent(ulong entityHandle, Type componentType)
        {
            return Entity_HasComponent(entityHandle, componentType);
        }
    }
}
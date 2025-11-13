using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace Razor
{
    internal static unsafe class InternalCalls
    {
        internal static delegate* unmanaged<ulong, Type, bool> Entity_HasComponent;
        internal static delegate* unmanaged[Cdecl]<IntPtr, void> Print_Message;
        internal static delegate* unmanaged<int*, uint*> Scene_GetEntitiesWithTransforms;

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

        public static List<UInt32> GetEntitiesWithTransforms()
        {
            int count;
            uint* ptr = Scene_GetEntitiesWithTransforms(&count);

            List<UInt32> entities = new List<UInt32>();
            for(int i = 0; i < count; i++)
            {
                entities.Add(ptr[i]);
            }

            return entities;
        }
    }
}
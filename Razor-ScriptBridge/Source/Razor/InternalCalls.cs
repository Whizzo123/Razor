using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace Razor
{
    internal static unsafe class InternalCalls
    {
        // <Params,..., return>
        internal static delegate* unmanaged<ulong, Type, bool> Entity_HasComponent;
        internal static delegate* unmanaged[Cdecl]<IntPtr, void> Print_Message;
        internal static delegate* unmanaged<int*, uint*> Scene_GetEntitiesWithTransforms;
        internal static delegate* unmanaged[Cdecl]<int, uint*, int*, int> Scene_GetEntitiesWithScriptComponent;
        internal static delegate* unmanaged[Cdecl]<sbyte*, int> Util_GetTypeIdForManagedType;
        internal static delegate* unmanaged[Cdecl]<int, uint, IntPtr> Scene_GetComponentOnEntity;
        internal static delegate* unmanaged[Cdecl]<int, int>                            Input_GetKey;
        internal static delegate* unmanaged[Cdecl]<uint, float*, float*, float*, void> Transform_GetPosition;
        internal static delegate* unmanaged[Cdecl]<uint, float, float, float, void>    Transform_SetPosition;
        internal static delegate* unmanaged[Cdecl]<uint, int>                          Collision_GetEventCount;
        internal static delegate* unmanaged[Cdecl]<uint, int, int*, uint*, void>       Collision_GetEvent;

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

        public static uint[] GetEntitiesWithComponent<T>()
        {
            string fullName = typeof(T).FullName;
            fixed (byte* name = Encoding.UTF8.GetBytes(fullName + '\0'))
            {
                int typeId = Util_GetTypeIdForManagedType((sbyte*)name);  // Get the Coral TypeId for T

                int count = 0;
                Scene_GetEntitiesWithScriptComponent(typeId, null, &count);

                if (count == 0)
                {
                    return Array.Empty<uint>();
                }

                uint[] result = new uint[count];
                fixed (uint* buf = result)
                {
                    int written = Scene_GetEntitiesWithScriptComponent(typeId, buf, &count); // Call C++ bridge
                    if (written != count)
                    {
                        Array.Resize(ref result, written);
                    }
                }

                return result;
            }
        }

        public static RazorKeyState GetInputKey(RazorKey key)
        {
            return (RazorKeyState)Input_GetKey((int)key);
        }

        public static (float X, float Y, float Z) GetTransformPosition(uint entityId)
        {
            float x, y, z;
            Transform_GetPosition(entityId, &x, &y, &z);
            return (x, y, z);
        }

        public static void SetTransformPosition(uint entityId, float x, float y, float z)
        {
            Transform_SetPosition(entityId, x, y, z);
        }

        public static int CollisionGetEventCount(uint entityId)
            => Collision_GetEventCount(entityId);

        public static CollisionEvent CollisionGetEvent(uint entityId, int index)
        {
            int type; uint otherId;
            Collision_GetEvent(entityId, index, &type, &otherId);
            return new CollisionEvent { Type = (CollisionEventType)type, OtherEntityId = otherId };
        }

        public static T GetComponent<T>(uint entityId) where T : Component
        {
            string fullName = typeof(T).FullName;
            fixed (byte* name = Encoding.UTF8.GetBytes(fullName + '\0'))
            {
                int typeId = Util_GetTypeIdForManagedType((sbyte*)name);  // Get the Coral TypeId for T

                IntPtr ptr = Scene_GetComponentOnEntity(typeId, entityId);
                if (ptr == IntPtr.Zero)
                    return default(T);

                GCHandle handle = GCHandle.FromIntPtr(ptr);
                object obj = handle.Target!;

                return obj as T;
            }
        }
    }
}
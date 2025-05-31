using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Razor
{
    public class Entity
    {
        public readonly ulong EntityHandle;

        public Entity() { }
        public Entity(ulong handle) { EntityHandle = handle; }

        public bool HasComponent<T>() where T : new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(EntityHandle, componentType);
        }
    }

}



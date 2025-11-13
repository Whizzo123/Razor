using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Razor
{
    public class Scene
    {
        public static List<UInt32> GetEntitiesWithTransforms()
        {
            return InternalCalls.GetEntitiesWithTransforms();
        }
    }
}

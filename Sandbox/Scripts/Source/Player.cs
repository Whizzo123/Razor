using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Razor;

namespace Sandbox
{
    public class Player : Razor.Component
    {
        public string Name;
        public float BaseSpeed = 5.0f;
        public int Direction;

        public float YDirection;
    }
}

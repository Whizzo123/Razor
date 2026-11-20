using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Razor;

namespace Sandbox
{
    public class ScoreSystem : Razor.System
    {
        public override void Run(float deltaTime)
        {
            var entities = Scene.GetEntitiesWithScriptComponent<Score>();
            Log logger = new Log();
            foreach (uint id in entities)
            {
                Score score = Scene.GetScriptComponent<Score>(id);
                Text text = Scene.GetNativeComponent<Text>(id);
                if (score == null || text == null) continue;
                var pos = Transform.GetPosition(id);

                text.SetText(score.score.ToString());
            }
        }
    }
}

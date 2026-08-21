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

		public static uint[] GetEntitiesWithScriptComponent<T>()
		{
			return InternalCalls.GetEntitiesWithComponent<T>();
		}

		public static T GetScriptComponent<T>(uint entity) where T : Component
		{
			return InternalCalls.GetComponent<T>(entity);
		}

		 public static List<uint> GetEntitiesWithNativeComponent<T>() where T : INativeComponent
 		{
			 if (typeof(T) == typeof(Text))
				 return InternalCalls.GetEntitiesWithText();
			 throw new NotImplementedException($"Native component type {typeof(T).Name} is not registered.");
 		}

 		public static T GetNativeComponent<T>(uint entityId) where T : INativeComponent, new()
 		{
			 if (typeof(T) == typeof(Text))
				 return (T)(object)new Text(entityId);
			 throw new NotImplementedException($"Native component type {typeof(T).Name} is not registered.");
 		}
	}
}

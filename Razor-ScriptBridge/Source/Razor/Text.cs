 namespace Razor
 {
     public interface INativeComponent { }

     public class Text : INativeComponent
     {
         private readonly uint _entityId;

         public Text() {}
         public Text(uint entityId) { _entityId = entityId; }

         public string GetText()                          => InternalCalls.TextGetText(_entityId);
         public void   SetText(string text)               => InternalCalls.TextSetText(_entityId, text);
         public void   SetColor(float r, float g, float b)=> InternalCalls.TextSetColor(_entityId, r, g, b);
         public void   SetScale(float scale)              => InternalCalls.TextSetScale(_entityId, scale);
     }
 }
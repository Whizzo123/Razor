namespace Razor
{
    public enum RazorKey
    {
        A = 0, D = 1, S = 2, W = 3,
        Escape = 4, Backspace = 5
    }

    public enum RazorKeyState
    {
        Released = 0, Pressed = 1, Held = 2
    }

    public static class Input
    {
        public static RazorKeyState GetKey(RazorKey key) => InternalCalls.GetInputKey(key);
        public static bool IsKeyPressed(RazorKey key)   => GetKey(key) != RazorKeyState.Released;
    }
}

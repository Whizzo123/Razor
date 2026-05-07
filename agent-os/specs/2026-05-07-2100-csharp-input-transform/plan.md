# C# Input & Transform — Plan

See full plan in `C:\Users\josep\.claude\plans\write-up-a-plan-jazzy-prism.md` (active plan file).

## Summary

Expose `RazorIO` key state and `Transform.Position` get/set to C# via three new internal calls, wrapped behind typed static `Input` and `Transform` classes.

## Tasks

1. Save spec documentation ✓
2. Add C++ internal call functions in `ScriptGlue.cpp` (`Input_GetKey`, `Transform_GetPosition`, `Transform_SetPosition`)
3. Add delegate declarations and low-level wrappers in `InternalCalls.cs`
4. Create `Input.cs` with `RazorKey`, `RazorKeyState` enums and `Input` static class
5. Create `Transform.cs` with `Transform` static class

## Files

| File | Change |
|---|---|
| `Razor/src/Razor/Scripting/ScriptGlue.cpp` | 3 new functions + registrations |
| `Razor-ScriptBridge/Source/Razor/InternalCalls.cs` | 3 delegate fields + 3 wrappers |
| `Razor-ScriptBridge/Source/Razor/Input.cs` | New |
| `Razor-ScriptBridge/Source/Razor/Transform.cs` | New |

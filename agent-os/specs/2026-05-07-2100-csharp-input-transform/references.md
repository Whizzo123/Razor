# References for C# Input & Transform

## Existing Internal Call Pattern

### Print_Message / LogMsg (simplest example)

- **C++ location:** `Razor/src/Razor/Scripting/ScriptGlue.cpp` — `extern "C"` block, `static void RAZOR_CALL Print_Message(const char* message)`
- **Registration:** `Assembly->AddInternalCall("Razor.InternalCalls", "Print_Message", (void*)Print_Message)`
- **C# delegate:** `internal static delegate* unmanaged[Cdecl]<IntPtr, void> Print_Message;`
- **C# wrapper:** `InternalCalls.LogMsg(string msg)` with `Marshal.StringToHGlobalAnsi`
- **Public API:** `Log.cs` → `InternalCalls.LogMsg(message)`
- **Relevance:** Canonical end-to-end template. `Input_GetKey` follows this exact shape.

### Scene_GetEntityIds (array return)

- **C++ location:** `Razor/src/Razor/Scripting/ScriptGlue.cpp`
- **C# delegate:** `internal static delegate* unmanaged[Cdecl]<IntPtr> Scene_GetEntityIds;`
- **Relevance:** Shows how entity IDs (`uint32_t` → `uint`) are passed across the bridge. The `uint` entity ID type used in `Transform_GetPosition`/`Transform_SetPosition` follows the same convention.

### Scene_GetComponentOnEntity (output pointer + GCHandle)

- **C++ location:** `Razor/src/Razor/Scripting/ScriptGlue.cpp` lines 84–101
- **Relevance:** Shows how managed objects are returned via `void*` / `IntPtr`. The Transform calls use a simpler float-pointer output approach instead of GCHandle, because `Transform` is a C++ struct not a managed object.

## Calling Convention

All internal call functions use `RAZOR_CALL` (`__cdecl`) and are declared in `extern "C"` blocks. C# delegates use `delegate* unmanaged[Cdecl]<...>`. This must match exactly or the bridge will silently misread parameters.

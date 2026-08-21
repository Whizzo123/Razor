# ImGui View Lifecycle

All editor views render via `RazorImGui::` — never call `ImGui::` directly. `RazorImGui` wraps ImGui to enforce consistent theming and allow backend swaps.

## Required structure for every Render()

```cpp
void MyView::Render()
{
    bool bIsOpen;  // required by RazorImGui::Begin — declare even if never read
    Razor::RazorImGui::Begin("My View", &bIsOpen);

    // ... render content using RazorImGui:: calls

    Razor::RazorImGui::End();
}
```

- `bIsOpen` is required boilerplate — declare it, do not remove it
- Pass `RazorGuiWindowFlags_MenuBar` as a third arg when the view has a menu bar
- Always pair every `Begin` with `End` — early returns must call `End` first

## Popups

Use `BeginPopupModal` for blocking workflows (new project, open project). Use `BeginPopup` for non-blocking overlays (add component). Do not mix the two for the same concept.

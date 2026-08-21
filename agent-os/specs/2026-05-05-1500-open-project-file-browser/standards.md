# Standards for Open Project File Browser

The following standards apply to this work.

---

## editor/imgui-view-lifecycle

All editor views render via `RazorImGui::` — never call `ImGui::` directly. `RazorImGui` wraps ImGui to enforce consistent theming and allow backend swaps.

### Required structure for every Render()

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

### Popups

Use `BeginPopupModal` for blocking workflows (new project, open project). Use `BeginPopup` for non-blocking overlays (add component). Do not mix the two for the same concept.

---

## editor/editor-storage

`EditorStorage` is the shared state bus for the Edge editor. It is passed as `Razor::Ref<EditorStorage>` to every view at startup.

- All editor state that crosses view boundaries lives here (e.g. `SelectedEntity`, project path)
- Engine-owned data (entities, components, scenes) stays in the engine layer — do not duplicate it in `EditorStorage`
- Every editor view receives `EditorStorage` — no exceptions

### Events

Use `MulticastDelegate` members on `EditorStorage` for cross-view notifications:

```cpp
// Declare in EditorStorage.h
OnProjectSetDelegate ProjectSetDelegate;
public:
  OnProjectSetDelegate& OnProjectSet() { return ProjectSetDelegate; }
  void SetProjectPath(const std::string& Proj) {
      _mCurrentProjectPath = Proj;
      ProjectSetDelegate.Broadcast();
  }

// Subscribe in EdgeApp.cpp or view constructor
Storage->OnProjectSet().AddRaw(this, &Edge::OnNewProjectSet);
```

- Never call view methods directly — broadcast events and let subscribers react
- Add new events as delegate members on `EditorStorage`, not as direct function calls

# EditorStorage

`EditorStorage` is the shared state bus for the Edge editor. It is passed as `Razor::Ref<EditorStorage>` to every view at startup.

- All editor state that crosses view boundaries lives here (e.g. `SelectedEntity`, project path)
- Engine-owned data (entities, components, scenes) stays in the engine layer — do not duplicate it in `EditorStorage`
- Every editor view receives `EditorStorage` — no exceptions

## Events

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

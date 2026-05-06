# Open Project File Browser

## Context

The current `OpenProjectPopupWindow` is a placeholder: it shows a modal with hardcoded "Create" and "Cancel" buttons and a TODO comment noting that a file explorer is needed. The "Create" button writes a hardcoded path (`/Sandbox/Sandbox`) to `EditorStorage`. This means the editor cannot actually open arbitrary projects. The goal is to replace this placeholder with a functional ImGui file browser so the user can navigate the filesystem, find a `.proj` file, and load it.

The browser must be cross-platform (no native OS dialogs), implemented entirely with `RazorImGui::` calls.

---

## Key Design Decisions

- **Starting path:** `std::filesystem::current_path().string()` — repo root on both Windows and Linux when launched per CLAUDE.md
- **Filter:** `GrabFiles()` returns only directories (for navigation) and `.proj` files (for selection)
- **Selection:** Click a `.proj` file → strip last 5 chars (`.proj`) → `Storage->SetProjectPath(strippedPath)`
- **Navigation:** Stack-based (`std::stack<std::string>`), Back guarded to size > 1, Cancel closes without loading
- **Layout:** 4-column `RazorImGui::BeginTable` grid, `ImageButton(nullptr, {100,100})` + label per entry

---

## Files Modified

| File | Change |
|------|--------|
| `Edge/src/Gui/OpenProjectPopupWindow.h` | Added `_mSearchStack`, `GrabFiles()`, `DrawFileGui()` |
| `Edge/src/Gui/OpenProjectPopupWindow.cpp` | Full rewrite — file browser replacing placeholder |

---

## Standards Applied

- `editor/imgui-view-lifecycle` — `RazorImGui::` only, `BeginPopupModal` + `EndPopup`, `bIsOpen` boilerplate
- `editor/editor-storage` — `Storage->SetProjectPath()` broadcasts delegate; no direct `Engine.LoadProject()` call

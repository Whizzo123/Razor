# Open Project File Browser — Shaping Notes

## Scope

Replace the placeholder `OpenProjectPopupWindow` (hardcoded path, Create/Cancel buttons) with a functional custom ImGui file browser. The user navigates the filesystem, finds a `.proj` file, clicks it, and the project loads.

## Decisions

- **Cross-platform ImGui browser** — no native OS dialogs (e.g. no Windows `IFileDialog`), so it works on Linux too
- **Starting path:** `std::filesystem::current_path()` — repo root on both Windows and Linux when Edge is launched per CLAUDE.md
- **Filter:** Show directories (navigation) + `.proj` files (selection) only; hide all other file types
- **Selection:** Click a `.proj` file → strip `.proj` suffix → call `Storage->SetProjectPath(strippedPath)`. `ProjectSerializer::Deserialize` appends `.proj` itself, so we strip it before passing.
- **Navigation:** Stack-based history (`std::stack<std::string>`), Back pops the stack (guarded to min size 1), Cancel closes without loading
- **Layout:** 4-column `RazorImGui::BeginTable` grid with `ImageButton` + label per entry — same pattern as `AssetPickerPopupWindow`
- **File extension:** `.proj` (not `.rzproj`) — matches current `ProjectSerializer` behavior

## Context

- **Visuals:** None provided
- **References:** `AssetPickerPopupWindow` (file traversal + ImGui grid pattern), `OpenProjectPopupWindow` (current placeholder being replaced)
- **Product alignment:** N/A — no `agent-os/product/` folder exists

## Standards Applied

- `editor/imgui-view-lifecycle` — use `RazorImGui::BeginPopupModal`, pair with `EndPopup`, `bIsOpen` boilerplate required
- `editor/editor-storage` — use `Storage->SetProjectPath()` to broadcast delegate; do not call `Engine.LoadProject()` directly

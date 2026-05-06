# References for Open Project File Browser

## Similar Implementations

### AssetPickerPopupWindow

- **Location:** `Edge/src/Gui/AssetPickerPopupWindow.h` and `.cpp`
- **Relevance:** Directly models the file browser we're building — same stack navigation, `GrabFiles()` with `std::filesystem::directory_iterator`, `DrawFileGui()` with `RazorImGui::ImageButton` in a 4-column table
- **Key patterns to borrow:**
  - `std::stack<std::string> _mSearchStack` for navigation history
  - `GrabFiles(path)` using `std::filesystem::directory_iterator` + `stat()` for directory detection
  - `DrawFileGui(file)` — directory click pushes to stack, file click sets selection and returns `true`
  - `ImageButton(nullptr, {100,100})` + `Text(label)` per entry layout
  - Back button guarded by `_mSearchStack.size() > 1`

### OpenProjectPopupWindow (current placeholder)

- **Location:** `Edge/src/Gui/OpenProjectPopupWindow.h` and `.cpp`
- **Relevance:** The file being replaced — inherit its class shape, `PopupWindow` base, `Open()`/`Close()` pattern, and `EditorStorage` usage
- **Key patterns to keep:**
  - Constructor accepting `Razor::Ref<EditorStorage>`
  - `Open()` calls `RazorImGui::OpenPopup(name)`
  - `Close()` calls `RazorImGui::CloseCurrentPopup()`
  - `Draw()` returns `bool bIsOpen`

### ProjectSerializer

- **Location:** `Razor/src/Razor/IO/File/ProjectSerializer.cpp:71`
- **Relevance:** Confirms that `Deserialize(path)` appends `.proj` internally (`PathPlusExt = Path + ".proj"`), so the path we pass to `Storage->SetProjectPath()` must NOT include the `.proj` extension

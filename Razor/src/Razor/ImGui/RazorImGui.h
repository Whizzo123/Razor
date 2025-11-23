#pragma once

#include "../Core.h"
#include "../Utils/Vector.h"
#include <memory>

typedef int ImGuiWindowFlags;
typedef int ImGuiDockNodeFlags;
typedef int ImGuiTreeNodeFlags;
typedef int ImGuiInputTextFlags;
typedef int ImGuiTextFlags;
typedef int ImGuiTableFlags;
typedef int ImGuiPopupFlags;

typedef unsigned int ImGuiID;

struct ImGuiViewport;

namespace Razor
{

	class IRenderer;
	class Window;
	class IWindowProvider;
	enum RazorMouseButton;
	enum RazorMouseState;

	enum RazorGuiWindowFlags_
	{
		RazorGuiWindowFlags_None = 0,
		RazorGuiWindowFlags_NoTitleBar = 1 << 0,   // Disable title-bar
		RazorGuiWindowFlags_NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
		RazorGuiWindowFlags_NoMove = 1 << 2,   // Disable user moving the window
		RazorGuiWindowFlags_NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
		RazorGuiWindowFlags_NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
		RazorGuiWindowFlags_NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
		RazorGuiWindowFlags_AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
		RazorGuiWindowFlags_NoBackground = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
		RazorGuiWindowFlags_NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
		RazorGuiWindowFlags_NoMouseInputs = 1 << 9,   // Disable catching mouse, hovering test with pass through.
		RazorGuiWindowFlags_MenuBar = 1 << 10,  // Has a menu-bar
		RazorGuiWindowFlags_HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
		RazorGuiWindowFlags_NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
		RazorGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
		RazorGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
		RazorGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
		RazorGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
		RazorGuiWindowFlags_NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
		RazorGuiWindowFlags_NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
		RazorGuiWindowFlags_UnsavedDocument = 1 << 20,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
		RazorGuiWindowFlags_NoDocking = 1 << 21,  // Disable docking of this window
		
		RazorGuiWindowFlags_NoNav = RazorGuiWindowFlags_NoNavInputs | RazorGuiWindowFlags_NoNavFocus,
		RazorGuiWindowFlags_NoDecoration = RazorGuiWindowFlags_NoTitleBar | RazorGuiWindowFlags_NoResize | RazorGuiWindowFlags_NoScrollbar | RazorGuiWindowFlags_NoCollapse,
		RazorGuiWindowFlags_NoInputs = RazorGuiWindowFlags_NoMouseInputs | RazorGuiWindowFlags_NoNavInputs | RazorGuiWindowFlags_NoNavFocus,
		
		RazorGuiWindowFlags_NavFlattened = 1 << 23,  // [BETA] On child window: allow gamepad/keyboard navigation to cross over parent border to this child or between sibling child windows.
		RazorGuiWindowFlags_ChildWindow = 1 << 24,  // Don't use! For internal use by BeginChild()
		RazorGuiWindowFlags_Tooltip = 1 << 25,  // Don't use! For internal use by BeginTooltip()
		RazorGuiWindowFlags_Popup = 1 << 26,  // Don't use! For internal use by BeginPopup()
		RazorGuiWindowFlags_Modal = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
		RazorGuiWindowFlags_ChildMenu = 1 << 28,  // Don't use! For internal use by BeginMenu()
		RazorGuiWindowFlags_DockNodeHost = 1 << 29,  // Don't use! For internal use by Begin()/NewFrame()
	};

	inline RazorGuiWindowFlags_ operator|(RazorGuiWindowFlags_ a, RazorGuiWindowFlags_ b)
	{
		return static_cast<RazorGuiWindowFlags_>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline RazorGuiWindowFlags_& operator|=(RazorGuiWindowFlags_& a, RazorGuiWindowFlags_ b)
	{
		a = a | b;
		return a;
	}

	// TODO API-CHANGE create seperate ImGUI opengl and dx11 api as shouldn't be having a Razor generic class containing a platform specific operation
	class RAZOR_API RazorImGui
	{
	public:
		RazorImGui() {}
		~RazorImGui() {}
		void Setup(const std::shared_ptr<IWindowProvider>& WindowProvider);
		void BeginNewFrame();
		void EndFrame(const Window& RenderWindow, Ref<IRenderer> Renderer);

		// ImGui API functions
		static void ShowMetricsWindow(bool* pOpen = nullptr);
		static void Begin(char const* Name, bool* pOpen = nullptr, ImGuiWindowFlags Flags = 0);
		static void End();
		static void SetNextWindowPos(Vector2 Pos);
		static void SetNextWindowSize(Vector2 Size);
		static void SetNextWindowViewport(ImGuiID Id);
		static Vector2 GetContentRegionAvail();
		static unsigned int GetID(const char * Title);
		static void Image(void* Texture, Vector2 const& ViewportSize, Vector2 const& UV0, Vector2 const& UV1, Vector4 const& TintColor = Vector4(1, 1, 1, 1), Vector4 const& BorderColor = Vector4(0, 0, 0, 0));
		static bool BeginMainMenuBar();
		static void EndMainMenuBar();
		static bool BeginMenuBar();
		static void EndMenuBar();
		static bool BeginMenu(const char* Name, bool Enabled = true);
		static void EndMenu();
		static bool MenuItem(const char* Name, const char* Shortcut = nullptr, bool Selected = false, bool Enabled = true);
		static unsigned int DockSpace(unsigned int ID, Vector2 Pos, ImGuiDockNodeFlags Flags = 0);
		static Vector2 GetItemRectMin();
		static unsigned int GetMainViewport();
		static bool InputFloat(const char* Label, float* Value, float Step = 0.0f, float StepFast = 0.0f, const char* Format = "%.3f", ImGuiInputTextFlags Flags = 0);
		static bool CollapsingHeader(const char* Label, ImGuiTreeNodeFlags Flags = 0);
		static void SameLine(float OffsetFromStartX = 0.0f, float Spacing = -1.0f);
		static bool Button(const char* Label, const Vector2& Size = Vector2(0, 0));
		static bool BeginPopupModal(const char* Name, bool* pOpen = nullptr, ImGuiWindowFlags Flags = 0);
		static void EndPopup();
		static void OpenPopup(const char* Name, ImGuiPopupFlags Flags = 0);
		static void CloseCurrentPopup();
		static void Text(const char* Text, ImGuiTextFlags Flags = 0);
		static bool TreeNode(const char* Label);
		static void TreePop();
		static bool BeginPopup(const char* Name, ImGuiWindowFlags Flags = 0);
		static bool BeginTable(const char* Name, int Columns, ImGuiTableFlags Flags = 0, const Vector2& OuterSize = Vector2(0, 0), int RowBgColor = -1);
		static void EndTable();
		static bool TableNextColumn();
		static bool ImageButton(void* TextureID, const Vector2& Size = Vector2(0, 0), const Vector2& UV0 = Vector2(0, 0), const Vector2& UV1 = Vector2(1, 1), int Framepadding = -1, const Vector4& BgColor = Vector4(0, 0, 0, 0), const Vector4& TintColor = Vector4(1, 1, 1, 1));
		static Vector2 GetViewportPos(unsigned int Id);
		static Vector2 GetViewportSize(unsigned int Id);
		static void SetWindowSize(const Vector2& Size);
		static bool Combo(const char* label, const char** data, int dataLength, int* selectedIdx);
		static bool InputText(const char* label, char* buf, size_t bufSize);
	public:
		// Function to hook our on such and such events to GLFW key events
		void RegisterImGuiEvents();
		void OnMouseButtonPressedEvent(RazorMouseButton Button, RazorMouseState State);
		bool OnMouseButtonReleasedEvent(RazorMouseButton Button, RazorMouseState State);
		void OnMouseMovedEvent(double XPos, double YPos);
		bool OnMouseScrolledEvent();
		
	private:
		static ImGuiViewport& GetViewport(unsigned int ID);
		float m_Time = 0.0f;
	};

}


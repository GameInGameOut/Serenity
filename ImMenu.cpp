#include "ImMenu.h"

namespace ImMenu
{
	bool D3Init = false;

	int iTab;
	void GuiInit(HWND window, IDirect3DDevice9 *pDevice)
	{
		ImGui_ImplDX9_Init(window, pDevice);
		auto& _style = ImGui::GetStyle();

		
		_style.Alpha = 1.0f;                                // Global alpha applies to everything in ImGui
		_style.WindowPadding = ImVec2(10, 10);                      // Padding within a window
		_style.WindowMinSize = ImVec2(100, 100);                    // Minimum window size
		_style.WindowRounding = 0.0f;                                // Radius of window corners rounding. Set to 0.0f to have rectangular windows
		_style.WindowTitleAlign = ImVec2(0.0f, 0.5f);                  // Alignment for title bar text
		//_style.ChildWindowRounding    = 0.0f;                                // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
		_style.FramePadding = ImVec2(5, 5);                        // Padding within a framed rectangle (used by most widgets)
		_style.FrameRounding = 0.0f;                                // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
		_style.ItemSpacing = ImVec2(5, 5);                        // Horizontal and vertical spacing between widgets/lines
		_style.ItemInnerSpacing = ImVec2(4, 4);                        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
		_style.TouchExtraPadding = ImVec2(0, 0);                        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
		_style.IndentSpacing = 21.0f;                               // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
		_style.ColumnsMinSpacing = 6.0f;                                // Minimum horizontal spacing between two columns
		_style.ScrollbarSize = 16.0f;                               // Width of the vertical scrollbar, Height of the horizontal scrollbar
		_style.ScrollbarRounding = 9.0f;                                // Radius of grab corners rounding for scrollbar
		_style.GrabMinSize = 10.0f;                               // Minimum width/height of a grab box for slider/scrollbar
		_style.GrabRounding = 0.0f;                                // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
		_style.ButtonTextAlign = ImVec2(0.5f, 0.5f);                  // Alignment of button text when button is larger than text.
		_style.DisplayWindowPadding = ImVec2(22, 22);                      // Window positions are clamped to be IsVisible within the display area by at least this amount. Only covers regular windows.
		_style.DisplaySafeAreaPadding = ImVec2(4, 4);                        // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
		_style.AntiAliasedLines = true;                                // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
		//_style.AntiAliasedShapes      = true;                                // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
		_style.CurveTessellationTol = 1.25f;                               // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.

		_style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		_style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		_style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		_style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		_style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
		_style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
		_style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		_style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		_style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		_style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		_style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		_style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		_style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		_style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		//_style.Colors[ImGuiCol_TitleText]            = ImVec4(0.80f, 0.80f, 1.00f, 1.00f);
		_style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		_style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		_style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
		_style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
		_style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
		//_style.Colors[ImGuiCol_ComboBg]              = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
		_style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.60f, 0.90f, 0.50f);
		_style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		_style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		_style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		_style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
		_style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
		_style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
		_style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		_style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		_style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		_style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
		_style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
		_style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		_style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		_style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		//_style.Colors[ImGuiCol_CloseButton]          = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
		//_style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
		//_style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
		_style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		_style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		_style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		_style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		_style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);


		D3Init = true;
	}

	ImFont* mainText;

	void Open() 
	{
		static bool bDown = false;
		static bool bClicked = false;
		static bool bPrevMenuState = g_Vars.menu.OpenedMenu;

		if (pressedKey[VK_INSERT])
		{
			bClicked = false;
			bDown = true;
		}
		else if (!pressedKey[VK_INSERT] && bDown)
		{
			bClicked = true;
			bDown = false;
		}
		else
		{
			bClicked = false;
			bDown = false;
		}

		if (bClicked)
		{
			g_Vars.menu.OpenedMenu = !g_Vars.menu.OpenedMenu;
		}

		if (bPrevMenuState != g_Vars.menu.OpenedMenu)
		{
		}

		bPrevMenuState = g_Vars.menu.OpenedMenu;
	}

	void ButtonColor(int r, int g, int b)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_Button] = ImColor(r, g, b);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(r, g, b);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(r, g, b);
	}

	void styled()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.f;
		style.FramePadding = ImVec2(4, 0);
		style.WindowPadding = ImVec2(0, 0);
		style.ItemSpacing = ImVec2(0, 0);
		style.ScrollbarSize = 10.f;
		style.ScrollbarRounding = 0.f;
		style.GrabMinSize = 5.f;
	}

	void color()
	{
		auto& _style = ImGui::GetStyle();

		
		_style.Alpha = 1.0f;                                // Global alpha applies to everything in ImGui
		_style.WindowPadding = ImVec2(10, 10);                      // Padding within a window
		_style.WindowMinSize = ImVec2(100, 100);                    // Minimum window size
		_style.WindowRounding = 0.0f;                                // Radius of window corners rounding. Set to 0.0f to have rectangular windows
		_style.WindowTitleAlign = ImVec2(0.0f, 0.5f);                  // Alignment for title bar text
		//_style.ChildWindowRounding    = 0.0f;                                // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
		_style.FramePadding = ImVec2(5, 5);                        // Padding within a framed rectangle (used by most widgets)
		_style.FrameRounding = 0.0f;                                // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
		_style.ItemSpacing = ImVec2(5, 5);                        // Horizontal and vertical spacing between widgets/lines
		_style.ItemInnerSpacing = ImVec2(4, 4);                        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
		_style.TouchExtraPadding = ImVec2(0, 0);                        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
		_style.IndentSpacing = 21.0f;                               // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
		_style.ColumnsMinSpacing = 6.0f;                                // Minimum horizontal spacing between two columns
		_style.ScrollbarSize = 16.0f;                               // Width of the vertical scrollbar, Height of the horizontal scrollbar
		_style.ScrollbarRounding = 9.0f;                                // Radius of grab corners rounding for scrollbar
		_style.GrabMinSize = 10.0f;                               // Minimum width/height of a grab box for slider/scrollbar
		_style.GrabRounding = 0.0f;                                // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
		_style.ButtonTextAlign = ImVec2(0.5f, 0.5f);                  // Alignment of button text when button is larger than text.
		_style.DisplayWindowPadding = ImVec2(22, 22);                      // Window positions are clamped to be IsVisible within the display area by at least this amount. Only covers regular windows.
		_style.DisplaySafeAreaPadding = ImVec2(4, 4);                        // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
		_style.AntiAliasedLines = true;                                // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
		//_style.AntiAliasedShapes      = true;                                // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
		_style.CurveTessellationTol = 1.25f;                               // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.

		_style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		_style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		_style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		_style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		_style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
		_style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
		_style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		_style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		_style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		_style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		_style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		_style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		_style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		_style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		//_style.Colors[ImGuiCol_TitleText]            = ImVec4(0.80f, 0.80f, 1.00f, 1.00f);
		_style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		_style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		_style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
		_style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
		_style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
		//_style.Colors[ImGuiCol_ComboBg]              = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
		_style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.60f, 0.90f, 0.50f);
		_style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		_style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		_style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		_style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
		_style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
		_style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
		_style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		_style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		_style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		_style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
		_style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
		_style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		_style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		_style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		//_style.Colors[ImGuiCol_CloseButton]          = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
		//_style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
		//_style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
		_style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		_style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		_style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		_style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		_style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}
	
	void Legit()
	{
		ImGui::BeginChild("Misc", ImVec2(700 / 1, 400), true);
		{
			ImGui::Spacing();
			ImGui::Checkbox("Backtracking", &g_Vars.legit.Backtrack);
			ImGui::Checkbox("1 Second Backtrack", &g_Vars.misc.FakeLatencyEnable);

			ImGui::Spacing();
			ImGui::Spacing();
		} ImGui::EndChild();
	}
	
	void Visuals()
	{
		static const char* snaplines[] = { "Off", "Bottom", "Center", "Top" };
		static const char* btchams[] = { "Off", "All Records", "LatestRecords" };

		ImGui::BeginChild("ESP", ImVec2(700 / 3, 400), true);
		{
			ImGui::Spacing();
			

			ImGui::Checkbox("Enemy Only", &g_Vars.visuals.EnemyOnly);

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Checkbox("Box", &g_Vars.visuals.Box);
			ImGui::Checkbox("Health", &g_Vars.visuals.Health);
			ImGui::Checkbox("Name", &g_Vars.visuals.Name);

			ImGui::Spacing();
			ImGui::Text("Snaplines");
			ImGui::Combo("", &g_Vars.visuals.Snaplines, snaplines, IM_ARRAYSIZE(snaplines));
			ImGui::Spacing();
			ImGui::Checkbox("Backtrack Dots", &g_Vars.visuals.BacktrackDots);
			ImGui::Text("Backtrack Chams");
			ImGui::Combo("", &g_Vars.visuals.BacktrackChams, btchams, IM_ARRAYSIZE(btchams));
		}
		ImGui::EndChild();

		ImGui::SameLine();

		static const char* chams[] = { "Off", "Reflective", "Flat" };

		ImGui::BeginChild("Misc", ImVec2(700 / 3, 400), true);
		{
			ImGui::Spacing();

			ImGui::Checkbox("Glow", &g_Vars.visuals.Glow);

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("Chams");
			ImGui::Combo("Chams Style", &g_Vars.visuals.ChamsStyle, chams, IM_ARRAYSIZE(chams));
			ImGui::Checkbox("Vis Only", &g_Vars.visuals.ChamsVisOnly);
		}
		ImGui::EndChild();
	}
	
	void Misc()
	{
		ImGui::BeginChild("Fake Latency", ImVec2(700 / 3, 400), true);
		{
			ImGui::Spacing();

			ImGui::Spacing();
		} ImGui::EndChild();
		
		ImGui::SameLine();

		ImGui::BeginChild("Movement", ImVec2(700 / 3, 400), true);
		{
			ImGui::Checkbox("Bhop", &g_Vars.misc.Bhop);
			ImGui::Spacing();
		} ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Other", ImVec2(700 / 3, 400), true);
		{
		} ImGui::EndChild();
	}

	void Draw() 
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.f;

		ImGui::SetNextWindowSize(ImVec2(700, 500));
		ImGui::SetNextWindowPosCenter(ImGuiSetCond_Always);

		color();

		ImGuiIO& io = ImGui::GetIO();

		

		io.MouseDrawCursor = true;

		ImGui::Begin("Project Serenity", &g_Vars.menu.OpenedMenu, ImVec2(700, 500), 0.4f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar);
		{
			style.ItemSpacing = ImVec2(0, 0);
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);				
			ImGui::Button("S E R E N I T Y", ImVec2(700, 30));

			int TabCount = 4;

			style.Colors[ImGuiCol_Text] = iTab == 0 ? ImVec4(0.961f, 0.961f, 0.961f, 1.00f) : ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			if (ImGui::Button("Legit", ImVec2(700 / TabCount, 70))) iTab = 0;
			ImGui::SameLine();
			style.Colors[ImGuiCol_Text] = iTab == 1 ? ImVec4(0.961f, 0.961f, 0.961f, 1.00f) : ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			if (ImGui::Button("Visuals", ImVec2(700 / TabCount, 70))) iTab = 1;
			style.Colors[ImGuiCol_Text] = iTab == 2 ? ImVec4(0.961f, 0.961f, 0.961f, 1.00f) : ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			ImGui::SameLine();
			if (ImGui::Button("Misc", ImVec2(700 / TabCount, 70))) iTab = 2;

			style.ItemSpacing = ImVec2(0, 5);

			switch (iTab)
			{
			case 0: //rage
				Legit();
				break;
			case 1: //rage
				Visuals();
				break;
			case 2: //rage
				Misc();
				break;
			}
		}

		ImGui::End();

		/*if (ImGui::Begin("Project Serenity", &g_Vars.menu.OpenedMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
		{
			ImGui::Checkbox("HvH", &g_Vars.menu.OpenedRage);
			ImGui::Checkbox("Legit", &g_Vars.menu.OpenedLegit);
			ImGui::Checkbox("Visuals", &g_Vars.menu.OpenedVisuals);
			ImGui::Checkbox("Misc", &g_Vars.menu.OpenedMisc);
		}
		ImGui::End();

		if (g_Vars.menu.OpenedRage)
		{
			if (ImGui::Begin("Rage", &g_Vars.menu.OpenedRage, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
			{
				ImGui::Checkbox("Enabled", &g_Vars.rage.Enabled);

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Checkbox("Recoil", &g_Vars.rage.RCS);
				ImGui::Checkbox("Multipoint", &g_Vars.rage.Multipoint);
				ImGui::Checkbox("Anti-Lagcomp", &g_Vars.rage.AntiLagcomp);
				ImGui::SliderFloat("Pointscale", &g_Vars.rage.Pointscale, 0, 1);

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::SliderInt("Min Damage", &g_Vars.rage.MinimumDamage, 0, 100);
				ImGui::SliderInt("Hitchance", &g_Vars.rage.Hitchance, 0, 100);

				ImGui::End();
			}

			if (ImGui::Begin("Hitboxes", &g_Vars.menu.OpenedRage, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
			{
				ImGui::Checkbox("Head", &g_Vars.rage.hitboxes.Head);
				ImGui::Checkbox("Chest", &g_Vars.rage.hitboxes.Chest);
				ImGui::Checkbox("Stomach", &g_Vars.rage.hitboxes.Stomach);
				ImGui::Checkbox("Pelvis", &g_Vars.rage.hitboxes.Pelvis);
				ImGui::Checkbox("Arms", &g_Vars.rage.hitboxes.Arms);
				ImGui::Checkbox("Feet", &g_Vars.rage.hitboxes.Feet);
			}

			if (ImGui::Begin("AntiAims", &g_Vars.menu.OpenedRage, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
			{
				static const char* pitch[] = { "Down", "Fake Down", "Up" };
				static const char* real[] = { "Arrow Key", "180 Treehouse" };

				ImGui::Checkbox("Enabled", &g_Vars.rage.antiaim.Enabled);

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Combo("Pitch", &g_Vars.rage.antiaim.Pitch, pitch, IM_ARRAYSIZE(pitch));
				ImGui::Combo("Real Yaw", &g_Vars.rage.antiaim.Real, real, IM_ARRAYSIZE(real));
			}
		}

		if (g_Vars.menu.OpenedLegit)
		{
			if (ImGui::Begin("Legit", &g_Vars.menu.OpenedLegit, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
			{
				ImGui::Checkbox("Backtracking", &g_Vars.legit.Backtrack);
				ImGui::SliderInt("Ticks", &g_Vars.legit.BacktrackTicks, 1, 12);

				ImGui::Spacing();
				ImGui::Spacing();


			}
			ImGui::End();
		}

		if (g_Vars.menu.OpenedVisuals)
		{
			if (ImGui::Begin("Visuals", &g_Vars.menu.OpenedVisuals, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
			{
				static const char* chams[] = { "Off", "Textured", "Flat", "Skeet" };
				static const char* snaplines[] = { "Off", "Center", "Bottom", "Top" };

				ImGui::Checkbox("Enabled", &g_Vars.visuals.Enabled);
				ImGui::Checkbox("Enemy Only", &g_Vars.visuals.EnemyOnly);

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Checkbox("ESP Box", &g_Vars.visuals.Box);
				ImGui::Text("Snaplines");
				ImGui::Combo("Snaplines", &g_Vars.visuals.Snaplines, snaplines, IM_ARRAYSIZE(snaplines));
				ImGui::Checkbox("ESP Name", &g_Vars.visuals.Name);
				ImGui::Checkbox("ESP Health", &g_Vars.visuals.Health);
				ImGui::Checkbox("ESP Weapon", &g_Vars.visuals.Weapon);
				ImGui::Checkbox("ESP Distance", &g_Vars.visuals.Distance);
				ImGui::Checkbox("ESP Money", &g_Vars.visuals.Money);

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Checkbox("Glow", &g_Vars.visuals.Glow);
				ImGui::Checkbox("Backtrack", &g_Vars.visuals.Backtrack);

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Chams");
				ImGui::Combo("Chams Style", &g_Vars.visuals.ChamsStyle, chams, IM_ARRAYSIZE(chams));
				ImGui::Checkbox("Vis Only", &g_Vars.visuals.ChamsVisOnly);
			}
			ImGui::End();
		}


		if (g_Vars.menu.OpenedMisc)
		{
			if (ImGui::Begin("Misc", &g_Vars.menu.OpenedMisc, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
			{
				ImGui::Checkbox("Bhop", &g_Vars.misc.Bhop);
			}
			ImGui::End();
		}*/

	}
}

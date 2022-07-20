#include <imgui.h>

class ImGuiContext {
public:
	inline ImGuiContext() {
		ImGui::CreateContext();
	}

	inline ~ImGuiContext() {
		ImGui::DestroyContext();
	}
};
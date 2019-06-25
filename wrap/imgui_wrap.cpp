#include "imgui_wrap.h"
#include "imgui/imgui.h"

#ifdef _WIN32
#include "glfw/glfw3.h"
#include "imgui_impl_glfw_gl3.h"

void imguiInit()
{
	extern GLFWwindow* gWindow;
	ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfwGL3_Init(gWindow, true);

	ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
}
void imguiNewFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
}
void imguiTerm()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}
void imguiRender() {
    ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}
#endif
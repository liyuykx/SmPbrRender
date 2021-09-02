#include "GUI.h"
constexpr auto SHADOWMAPPING = 0;
constexpr auto PBR = 1;

GUI::GUI(WindowManager& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ExampleID = 1;
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}
void GUI::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void GUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::draw() {
    

    ImGui::SetNextWindowBgAlpha(1.0);//setup transparent
    {
        ImGui::Begin("feature");

        ImGui::RadioButton("ShadowMapping", &ExampleID, SHADOWMAPPING);
        ImGui::RadioButton("PBR", &ExampleID, PBR);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
int GUI::getExampleID() {
    return ExampleID;
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

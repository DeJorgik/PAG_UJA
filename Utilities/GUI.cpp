//
// Created by DeJorgiK on 20/09/2024.
//

#include "GUI.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

namespace PAG {

    PAG::GUI* PAG::GUI::gui_instance = nullptr;

    GUI::GUI() {

    }

    GUI::~GUI() {

    }

    GUI &GUI::getInstance() {
        if(!gui_instance){
            gui_instance = new GUI;
        }
        return *gui_instance;
    }

    void GUI::guiInit() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    }

    void GUI::shutDown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext ();
    }

    void GUI::newFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUI::render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUI::drawMessage(float posX,float posY,float fontScale,const char *title,const char *text) {
        ImGui::SetNextWindowPos(ImVec2 (posX, posY), ImGuiCond_Once );
        if ( ImGui::Begin (title)){
            ImGui::SetWindowFontScale (fontScale);
            ImGui::TextUnformatted(text);
        }
        ImGui::End ();
    }

    void GUI::drawColorWheel(float posX,float posY,float fontScale,float *col,const char *title,const char *text) {
        ImGui::SetNextWindowPos ( ImVec2 (posX, posY), ImGuiCond_Once );
        if (ImGui::Begin(title)){
            ImGui::SetWindowFontScale ( fontScale );
            ImGui::ColorPicker3(text,col,ImGuiColorEditFlags_PickerHueWheel);
        }
        ImGui::End();
    }

} // PAG
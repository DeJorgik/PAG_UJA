//
// Created by DeJorgiK on 20/09/2024.
//

#include <string>
#include "GUI.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_stdlib.h"

namespace PAG {

    PAG::GUI* PAG::GUI::gui_instance = nullptr;

    GUI::GUI() {
        shaderLoadButtonPressed = false;
        shaderLoadInputText = "";
    }

    GUI::~GUI() {

    }

    GUI &GUI::getInstance() {
        if(!gui_instance){
            gui_instance = new GUI;
        }
        return *gui_instance;
    }

    void GUI::guiInit(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window,true); //Preguntar si es asi o no???
        ImGui_ImplOpenGL3_Init();
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

    void GUI::drawShaderLoadWindow(float posX, float posY, float fontScale, const char *title) {
        ImGui::SetNextWindowPos ( ImVec2 (posX, posY), ImGuiCond_Once );
        if (ImGui::Begin(title)){
            ImGui::SetWindowFontScale ( fontScale );
            ImGui::InputText ( "##", &shaderLoadInputText, ImGuiInputTextFlags_AutoSelectAll );
            shaderLoadButtonPressed = ImGui::Button("Load");
        }
        ImGui::End();
    }

    bool GUI::isShaderLoadButtonPressed() const {
        return shaderLoadButtonPressed; //devuelve cuado el botón esta presionado para cargar un nuevo shader
    }

    const std::string &GUI::getShaderLoadInputText() const {
        return shaderLoadInputText;
    }

    void GUI::messageBufferAdd(std::string text) {
        messageBuffer<<text<<std::endl;
        //Si se pasa del tamaño maximo se reinicia
        if(messageBuffer.str().size()==messageBuffer.str().max_size()){
            messageBuffer.str(std::string());
        }

    }

    std::string GUI::getMessageBufferText() {
        return messageBuffer.str();
    }

} // PAG
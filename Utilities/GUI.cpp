//
// Created by DeJorgiK on 20/09/2024.
//

#include <string>
#include "GUI.h"

#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_stdlib.h"

namespace PAG {

    PAG::GUI* PAG::GUI::gui_instance = nullptr;

    GUI::GUI() {
        shaderLoadButtonPressed = false;
        shaderLoadInputText = "";
        cameraControlSelectedItem = 0;
        cameraZoomValue = 60.f;
        dollyForwardPressed = false;
        dollyBackwardPressed = false;
        dollyLeftPressed = false;
        dollyRightPressed = false;
        panAngle = 0.0f;
        fileBrowserWindow.SetPwd("../Models");
        currentModelIndex = 0;
        modelTransformSelectedItem = 0;
        modelTranslate = glm::vec3(0.f);
        modelRotateAxis = glm::vec3(0,1,0);
        modelRotateAngle = .0f;
        modelScale = glm::vec3(1,1,1);
        modelTransformApplyPressed = false;
        modelDeletePressed = false;
        modelVisualizationTypeFillPressed = true;
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

    void GUI::drawCameraControls(float posX, float posY, float fontScale, const char *title) {
        ImGui::SetNextWindowPos ( ImVec2 (posX, posY), ImGuiCond_Once );
        if (ImGui::Begin(title)){
            ImGui::SetWindowFontScale ( fontScale );
            const char* items[] = {"Zoom", "Pan", "Tilt","Dolly","Crane","Orbit"};
            ImGui::Combo("Controls", &cameraControlSelectedItem, items, 6);

            switch (cameraControlSelectedItem) {
                case 0:
                    cameraMovement = PAG::cameraMovementType::ZOOM;
                    ImGui::SliderFloat("Zoom",&cameraZoomValue,1.f,120.f);
                    break;
                case 1:
                    cameraMovement = PAG::cameraMovementType::PAN;
                    ImGui::SliderFloat("Pan",&panAngle,-0.1f,0.1f);
                    if (!ImGui::IsItemActive()) {
                        panAngle = 0.0f;
                    }
                    break;
                case 2:
                    cameraMovement = PAG::cameraMovementType::TILT;
                    ImGui::SliderFloat("Tilt",&tiltAngle,-0.1f,0.1f);
                    if (!ImGui::IsItemActive()) {
                        tiltAngle = 0.0f;
                    }
                    break;
                case 3:
                    cameraMovement = PAG::cameraMovementType::DOLLY;
                    dollyForwardPressed = ImGui::Button("Forward");
                    ImGui::SameLine();
                    dollyBackwardPressed = ImGui::Button("Backward");
                    dollyLeftPressed = ImGui::Button("Left");
                    ImGui::SameLine();
                    dollyRightPressed = ImGui::Button("Right");
                    break;
                case 4:
                    cameraMovement = PAG::cameraMovementType::CRANE;
                    craneUpPressed = ImGui::Button("Up");
                    ImGui::SameLine();
                    craneDownPressed = ImGui::Button("Down");
                    break;
                case 5:
                    cameraMovement = PAG::cameraMovementType::ORBIT;
                    ImGui::SliderFloat("Longitude",&longitudeAngle,-0.1f,0.1f);
                    if (!ImGui::IsItemActive()) {
                        longitudeAngle = 0.0f;
                    }
                    ImGui::SliderFloat("Latitude",&latitudeAngle,-0.1f,0.1f);
                    if (!ImGui::IsItemActive()) {
                        latitudeAngle = 0.0f;
                    }
                    break;
                default:
                    break;
            }
        }
        ImGui::End();
    }

    //Ventana de carga de modelo
    void GUI::drawModelLoaderWindow(float posX, float posY, float fontScale, const char *title){
        ImGui::SetNextWindowPos ( ImVec2 (posX, posY), ImGuiCond_Once );
        if (ImGui::Begin(title)){
            ImGui::SeparatorText("Shader");
            ImGui::SetWindowFontScale ( fontScale );
            ImGui::InputText ( "##", &shaderLoadInputText, ImGuiInputTextFlags_AutoSelectAll );
            ImGui::SeparatorText("Material");
            ImGui::ColorPicker3("Ambient Color",modelAmbientColor,ImGuiColorEditFlags_PickerHueWheel);
            ImGui::Checkbox("Fill Model",&modelVisualizationTypeFillPressed);
            ImGui::SeparatorText("Model");
            if(ImGui::Button("Select Model")){
                fileBrowserWindow.Open();    //Abrir ventana al pulsar el botón
            }
        }
        ImGui::End();
        fileBrowserWindow.Display();
    }

    //Función para borrar el buffer del file browser
    void GUI::clearModelLoader(){
        fileBrowserWindow.ClearSelected();
    }

    /**
     * Función que lista los modelos que actualmente están en la escena, al igual que
     * las operaciones que se pueden realizar con estos
     * Contiene un puntero a la lista con los modelos del Renderer
     */
    void GUI::drawModelTransformWindow(float posX, float posY, float fontScale, const char *title,
                                       std::vector<std::pair<PAG::Model,GLuint>>* modelList){
        ImGui::SetNextWindowPos ( ImVec2 (posX, posY), ImGuiCond_Once );
        if (ImGui::Begin(title)){
            ImGui::SeparatorText("Model Transform");
            if(!modelList->empty()){
                if(ImGui::BeginCombo("Model List",modelList->at(currentModelIndex).first.getModelName()->c_str())){
                    for (int i = 0; i < modelList->size(); ++i) {
                        bool isSelected = currentModelIndex == i;
                        if (ImGui::Selectable(modelList->at(i).first.getModelName()->c_str(), isSelected)){
                            currentModelIndex = i;
                        }
                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
            } else {
                ImGui::Text("No models loaded.");
            }
            const char* items[] = {"Translate", "Rotate", "Scale"};
            ImGui::Combo("Transformation", &modelTransformSelectedItem, items, 3);
            switch (modelTransformSelectedItem) {
                case 0:
                    modelTransform = PAG::modelTransformType::TRANSLATE;
                    ImGui::InputFloat("X", &modelTranslate.x);
                    ImGui::InputFloat("Y", &modelTranslate.y);
                    ImGui::InputFloat("Z", &modelTranslate.z);
                    break;
                case 1:
                    modelTransform = PAG::modelTransformType::ROTATE;
                    ImGui::InputFloat("X Axis", &modelRotateAxis.x);
                    ImGui::InputFloat("Y Axis", &modelRotateAxis.y);
                    ImGui::InputFloat("Z Axis", &modelRotateAxis.z);
                    ImGui::InputFloat("Angle", &modelRotateAngle);
                    break;
                case 2:
                    modelTransform = PAG::modelTransformType::SCALE;
                    ImGui::InputFloat("X", &modelScale.x);
                    ImGui::InputFloat("Y", &modelScale.y);
                    ImGui::InputFloat("Z", &modelScale.z);
                    break;
                default:
                    break;
            }
            modelTransformApplyPressed = ImGui::Button("Apply");
            ImGui::SameLine();
            if(ImGui::Button("Reset")){
                modelTranslate = glm::vec3(0.f);
                modelRotateAxis = glm::vec3(0,1,0);
                modelRotateAngle = .0f;
                modelScale = glm::vec3(1,1,1);
            }
            ImGui::SeparatorText("Model Manegment");
            modelDeletePressed = ImGui::Button("Delete Model");
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

    cameraMovementType GUI::getCameraMovement() const {
        return cameraMovement;
    }

    float GUI::getCameraZoomValue() const {
        return cameraZoomValue;
    }

    bool GUI::isDollyForwardPressed() const {
        return dollyForwardPressed;
    }

    bool GUI::isDollyBackwardPressed() const {
        return dollyBackwardPressed;
    }

    bool GUI::isDollyLeftPressed() const {
        return dollyLeftPressed;
    }

    bool GUI::isDollyRightPressed() const {
        return dollyRightPressed;
    }

    float GUI::getPanAngle() const {
        return panAngle;
    }

    float GUI::getTiltAngle() const {
        return tiltAngle;
    }

    bool GUI::isCraneUpPressed() const {
        return craneUpPressed;
    }

    bool GUI::isCraneDownPressed() const {
        return craneDownPressed;
    }

    float GUI::getLongitudeAngle() const {
        return longitudeAngle;
    }

    float GUI::getLatitudeAngle() const {
        return latitudeAngle;
    }

    const ImGui::FileBrowser &GUI::getFileBrowserWindow() const {
        return fileBrowserWindow;
    }

    bool GUI::isModelTransformApplyPressed() const {
        return modelTransformApplyPressed;
    }

    int GUI::getCurrentModelIndex() const {
        return currentModelIndex;
    }

    int GUI::getModelTransformSelectedItem() const {
        return modelTransformSelectedItem;
    }

    modelTransformType GUI::getModelTransform() const {
        return modelTransform;
    }

    const glm::vec3 &GUI::getModelTranslate() const {
        return modelTranslate;
    }

    const glm::vec3 &GUI::getModelRotateAxis() const {
        return modelRotateAxis;
    }

    float GUI::getModelRotateAngle() const {
        return modelRotateAngle;
    }

    const glm::vec3 &GUI::getModelScale() const {
        return modelScale;
    }

    bool GUI::isModelDeletePressed() const {
        return modelDeletePressed;
    }

    void GUI::setCurrentModelIndex(int currentModelIndex) {
        GUI::currentModelIndex = currentModelIndex;
    }

    modelVisualizationTypes GUI::getModelVisualizationType() const {
        if(modelVisualizationTypeFillPressed) return modelVisualizationTypes::FILL;
        else return modelVisualizationTypes::WIREFRAME;
    }

    const float *GUI::getModelAmbientColor() const {
        return modelAmbientColor;
    }


} // PAG

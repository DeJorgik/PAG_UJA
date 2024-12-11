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
        createLightPressed = false;
        createLightSelectedItem = 0;
        currentLightIndex = 0;
        deleteLightPressed = false;
        modelMaterialApplyPressed = false;
        editLightPressed = false;
        createModelVisualizationTypeIndex = 0;
        editModelVisualizationTypeIndex = 0;
    }

    GUI::~GUI() {

    }

    //INICIALIZACIÓN

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

    //VENTANAS

    void GUI::drawMessageWindow(float posX,float posY,float fontScale){
        ImGui::SetNextWindowPos(ImVec2 (posX, posY), ImGuiCond_Once );
        if ( ImGui::Begin ("Messages")){
            ImGui::SetWindowFontScale (fontScale);
            ImGui::TextUnformatted(getMessageBufferText().c_str());
        }
        ImGui::End ();
    }

    /**
     * Ventana principal que alberga todos los controles de la aplicación
     */
    void GUI::drawControlWindow(float posX,float posY,float fontScale,
                                std::vector<std::pair<PAG::Model,GLuint>>* modelList,
                                std::vector<PAG::Light>* lightList,
                                float *col,
                                int lightCount){
        ImGui::SetNextWindowPos(ImVec2 (posX, posY), ImGuiCond_Once );
        if ( ImGui::Begin ("Controls")){
            ImGui::SetWindowFontScale (fontScale);
            if(ImGui::BeginTabBar("")){

                //Control Modelos
                if (ImGui::BeginTabItem("Models")){
                    if(ImGui::TreeNode("Model Load")){
                        ImGui::SeparatorText("Shader");
                        ImGui::InputText ( "##", &shaderLoadInputText, ImGuiInputTextFlags_AutoSelectAll );
                        ImGui::SeparatorText("Visualization Type");
                        ImGui::RadioButton("Texture", &createModelVisualizationTypeIndex, 0); ImGui::SameLine();
                        ImGui::RadioButton("Material", &createModelVisualizationTypeIndex, 1); ImGui::SameLine();
                        ImGui::RadioButton("Wireframe", &createModelVisualizationTypeIndex, 2);

                        switch (createModelVisualizationTypeIndex) {
                            case 0:
                                ImGui::Text("Poner para cargar");
                                break;
                            case 1:
                                ImGui::SeparatorText("Material");
                                ImGui::ColorEdit3("Ambient Color",modelAmbientColor);
                                ImGui::ColorEdit3("Diffuse Color",modelDiffuseColor);
                                ImGui::ColorEdit3("Specular Color",modelSpecularColor);
                                ImGui::InputFloat("Specular Exponent", &modelSpecularExponent);
                                break;
                            case 2:
                                break;
                        }

                        ImGui::SeparatorText("Model");
                        if(ImGui::Button("Select Model")){
                            fileBrowserWindow.Open();    //Abrir ventana al pulsar el botón
                        }
                        fileBrowserWindow.Display();
                        ImGui::TreePop();
                    }
                    if(ImGui::TreeNode("Model Edit")){
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

                        ImGui::SeparatorText("Visualization Type");
                        ImGui::RadioButton("Texture", &editModelVisualizationTypeIndex, 0); ImGui::SameLine();
                        ImGui::RadioButton("Material", &editModelVisualizationTypeIndex, 1); ImGui::SameLine();
                        ImGui::RadioButton("Wireframe", &editModelVisualizationTypeIndex, 2);
                        switch (editModelVisualizationTypeIndex) {
                            case 0:
                                ImGui::Text("Poner para cargar");
                                break;
                            case 1:
                                ImGui::SeparatorText("Material Edit");
                                ImGui::ColorEdit3("Ambient Color",modelAmbientColorTransform);
                                ImGui::ColorEdit3("Diffuse Color",modelDiffuseColorTransform);
                                ImGui::ColorEdit3("Specular Color",modelSpecularColorTransform);
                                ImGui::InputFloat("Specular Exponent", &modelSpecularExponentTransform);
                                break;
                            case 2:
                                break;
                        }

                        //Aparte para cambiar material

                        modelMaterialApplyPressed = ImGui::Button("Apply Visualization Type");

                        ImGui::SeparatorText("Model Delete");
                        modelDeletePressed = ImGui::Button("Delete Model");
                        ImGui::TreePop();
                    }
                    ImGui::EndTabItem();
                }

                //Control Luces
                if(ImGui::BeginTabItem("Lights")){
                    if(ImGui::TreeNode("Light Add")){
                        ImGui::SeparatorText("Create Light");
                        const char* items[] = {"Ambient", "Direction", "Point", "Spot"};
                        ImGui::Combo("Light Type", &createLightSelectedItem, items, 4);
                        switch (createLightSelectedItem) {
                            case 0:
                                createLightType = PAG::lightTypes::AMBIENT;
                                ImGui::ColorEdit3("Ambient Color",lightAmbientColor);
                                break;
                            case 1:
                                createLightType = PAG::lightTypes::DIRECTION;
                                ImGui::ColorEdit3("Diffuse Color",lightDiffuseColor);
                                ImGui::ColorEdit3("Specular Color",lightSpecularColor);
                                ImGui::InputFloat("X Direction", &lightDirection.x);
                                ImGui::InputFloat("Y Direction", &lightDirection.y);
                                ImGui::InputFloat("Z Direction", &lightDirection.z);
                                break;
                            case 2:
                                createLightType = PAG::lightTypes::POINT;
                                ImGui::ColorEdit3("Diffuse Color",lightDiffuseColor);
                                ImGui::ColorEdit3("Specular Color",lightSpecularColor);
                                ImGui::InputFloat("X Position", &lightPosition.x);
                                ImGui::InputFloat("Y Position", &lightPosition.y);
                                ImGui::InputFloat("Z Position", &lightPosition.z);
                                break;
                            case 3:
                                createLightType = PAG::lightTypes::SPOT;
                                ImGui::ColorEdit3("Diffuse Color",lightDiffuseColor);
                                ImGui::ColorEdit3("Specular Color",lightSpecularColor);
                                ImGui::InputFloat("X Position", &lightPosition.x);
                                ImGui::InputFloat("Y Position", &lightPosition.y);
                                ImGui::InputFloat("Z Position", &lightPosition.z);
                                ImGui::InputFloat("X Direction", &lightDirection.x);
                                ImGui::InputFloat("Y Direction", &lightDirection.y);
                                ImGui::InputFloat("Z Direction", &lightDirection.z);
                                ImGui::SliderFloat("Gamma",&lightGamma,0.f,90.f);

                                //ImGui::InputFloat("Smooth factor", &lightS);
                                break;
                            default:
                                break;
                        }
                        createLightPressed = ImGui::Button("Create Light");
                        ImGui::TreePop();
                    }
                    if(ImGui::TreeNode("Light Edit")){
                        ImGui::SeparatorText("Edit Light");
                        if(ImGui::BeginCombo("Light List",lightList->at(currentLightIndex).getLightName().c_str())){
                            for (int i = 0; i < lightList->size(); ++i) {
                                bool isSelected = currentLightIndex == i;
                                if (ImGui::Selectable(lightList->at(i).getLightName().c_str(), isSelected)){
                                    currentLightIndex = i;
                                }
                                if (isSelected) {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                            ImGui::EndCombo();
                        }
                        //Editar luz
                        switch (lightList->at(currentLightIndex).getLightType()) {
                            case 0:
                                ImGui::ColorEdit3("Ambient Color",lightAmbientColorEdit);
                                break;
                            case 1:
                                ImGui::ColorEdit3("Diffuse Color",lightDiffuseColorEdit);
                                ImGui::ColorEdit3("Specular Color",lightSpecularColorEdit);
                                ImGui::InputFloat("X Direction", &lightDirectionEdit.x);
                                ImGui::InputFloat("Y Direction", &lightDirectionEdit.y);
                                ImGui::InputFloat("Z Direction", &lightDirectionEdit.z);
                                break;
                            case 2:
                                ImGui::ColorEdit3("Diffuse Color",lightDiffuseColorEdit);
                                ImGui::ColorEdit3("Specular Color",lightSpecularColorEdit);
                                ImGui::InputFloat("X Position", &lightPositionEdit.x);
                                ImGui::InputFloat("Y Position", &lightPositionEdit.y);
                                ImGui::InputFloat("Z Position", &lightPositionEdit.z);
                                break;
                            case 3:
                                ImGui::ColorEdit3("Diffuse Color",lightDiffuseColorEdit);
                                ImGui::ColorEdit3("Specular Color",lightSpecularColorEdit);
                                ImGui::InputFloat("X Position", &lightPositionEdit.x);
                                ImGui::InputFloat("Y Position", &lightPositionEdit.y);
                                ImGui::InputFloat("Z Position", &lightPositionEdit.z);
                                ImGui::InputFloat("X Direction", &lightDirectionEdit.x);
                                ImGui::InputFloat("Y Direction", &lightDirectionEdit.y);
                                ImGui::InputFloat("Z Direction", &lightDirectionEdit.z);
                                ImGui::SliderFloat("Gamma",&lightGammaEdit,0.f,90.f);

                                //ImGui::InputFloat("Smooth factor", &lightSEdit);
                                break;
                            default:
                                break;
                        }

                        editLightPressed = ImGui::Button("Apply");

                        //Borrar luz
                        ImGui::SeparatorText("Delete Light");
                        if(lightCount>1) {
                            deleteLightPressed = ImGui::Button("Delete Light");
                        }else{
                            deleteLightPressed = false;
                            ImGui::Text("There has to be at least one light.");
                        }
                        ImGui::TreePop();
                    }
                    ImGui::EndTabItem();
                }

                //Control Cámara
                if(ImGui::BeginTabItem("Camera")){
                    ImGui::SeparatorText("Camera Controls");

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
                    ImGui::EndTabItem();
                }

                //Color del fondo
                if(ImGui::BeginTabItem("Background")){
                    ImGui::SeparatorText("Background Color");
                    ImGui::ColorPicker3("Current",col,ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End ();
    }

    //Función para borrar el buffer del file browser
    void GUI::clearModelLoader(){
        fileBrowserWindow.ClearSelected();
    }

    //GETTERS Y SETTERS

    GUI &GUI::getInstance() {
        if(!gui_instance){
            gui_instance = new GUI;
        }
        return *gui_instance;
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
        switch (createModelVisualizationTypeIndex) {
            case 0:
                return modelVisualizationTypes::TEXTURED;
                break;
            case 1:
                return modelVisualizationTypes::FILL;
                break;
            case 2:
                return modelVisualizationTypes::WIREFRAME;
                break;
        }
    }

    modelVisualizationTypes GUI::getEditModelVisualizationType() const {
        switch (editModelVisualizationTypeIndex) {
            case 0:
                return modelVisualizationTypes::TEXTURED;
                break;
            case 1:
                return modelVisualizationTypes::FILL;
                break;
            case 2:
                return modelVisualizationTypes::WIREFRAME;
                break;
        }
    }

    const float *GUI::getModelAmbientColor() const {
        return modelAmbientColor;
    }

    const float *GUI::getModelAmbientColorTransform() const {
        return modelAmbientColorTransform;
    }

    bool GUI::isCreateLightPressed() const {
        return createLightPressed;
    }

    bool GUI::isDeleteLightPressed() const {
        return deleteLightPressed;
    }

    lightTypes GUI::getCreateLightType() const {
        return createLightType;
    }

    const float *GUI::getLightAmbientColor() const {
        return lightAmbientColor;
    }

    const float *GUI::getLightDiffuseColor() const {
        return lightDiffuseColor;
    }

    const float *GUI::getLightSpecularColor() const {
        return lightSpecularColor;
    }

    const glm::vec3 &GUI::getLightPosition() const {
        return lightPosition;
    }

    const glm::vec3 &GUI::getLightDirection() const {
        return lightDirection;
    }

    float GUI::getLightGamma() const {
        return lightGamma;
    }

    float GUI::getLightS() const {
        return lightS;
    }

    const float *GUI::getModelDiffuseColor() const {
        return modelDiffuseColor;
    }

    const float *GUI::getModelSpecularColor() const {
        return modelSpecularColor;
    }

    float GUI::getModelSpecularExponent() const {
        return modelSpecularExponent;
    }

    const float *GUI::getModelDiffuseColorTransform() const {
        return modelDiffuseColorTransform;
    }

    const float *GUI::getModelSpecularColorTransform() const {
        return modelSpecularColorTransform;
    }

    float GUI::getModelSpecularExponentTransform() const {
        return modelSpecularExponentTransform;
    }

    int GUI::getCurrentLightIndex() const {
        return currentLightIndex;
    }

    void GUI::setCurrentLightIndex(int currentLightIndex) {
        GUI::currentLightIndex = currentLightIndex;
    }

    bool GUI::isModelMaterialApplyPressed() const {
        return modelMaterialApplyPressed;
    }

    const float *GUI::getLightAmbientColorEdit() const {
        return lightAmbientColorEdit;
    }

    const float *GUI::getLightDiffuseColorEdit() const {
        return lightDiffuseColorEdit;
    }

    const float *GUI::getLightSpecularColorEdit() const {
        return lightSpecularColorEdit;
    }

    const glm::vec3 &GUI::getLightPositionEdit() const {
        return lightPositionEdit;
    }

    const glm::vec3 &GUI::getLightDirectionEdit() const {
        return lightDirectionEdit;
    }

    float GUI::getLightGammaEdit() const {
        return lightGammaEdit;
    }

    float GUI::getLightSEdit() const {
        return lightSEdit;
    }

    bool GUI::isEditLightPressed() const {
        return editLightPressed;
    }


} // PAG

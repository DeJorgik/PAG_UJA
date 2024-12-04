//
// Created by DeJorgiK on 20/09/2024.
//

#ifndef PAG_1_GUI_H
#define PAG_1_GUI_H

#include <sstream>
#include "Camera.h"
#include "../imgui/imfilebrowser.h"
#include "Model.h"
#include "Light.h"

namespace PAG {

    class GUI {
    private:
        static GUI* gui_instance;
        GUI();
        std::string shaderLoadInputText; //Texto con el nombre del shader
        std::stringstream messageBuffer; //Buffer de los mensajes que salen por pantalla
        int cameraControlSelectedItem; //Control elegido en el combo de la camara
        PAG::cameraMovementType cameraMovement; //Que tipo de movimiento está realizando la cámara
        float cameraZoomValue; //valor de zoom de la cámara;
        bool dollyForwardPressed;
        bool dollyBackwardPressed;
        bool dollyLeftPressed;
        bool dollyRightPressed;
        float panAngle;
        float tiltAngle;
        bool craneUpPressed;
        bool craneDownPressed;
        float longitudeAngle;
        float latitudeAngle;
        ImGui::FileBrowser fileBrowserWindow;
        //PRÁCTICA 6
        int currentModelIndex; //Indice del modelo actual
        int modelTransformSelectedItem;
        PAG::modelTransformType modelTransform;
        glm::vec3 modelTranslate;
        glm::vec3 modelRotateAxis;
        float modelRotateAngle;
        glm::vec3 modelScale;
        bool modelTransformApplyPressed;
        bool modelMaterialApplyPressed;
        bool modelDeletePressed;
        //Material
        bool modelVisualizationTypeFillPressed;
        float modelAmbientColor[3] = {0,0,0};
        float modelDiffuseColor[3] = {1,0,0};
        float modelSpecularColor[3] = {1,1,1};
        float modelSpecularExponent = 1.5;
        float modelAmbientColorTransform[3] = {0,0,0};
        float modelDiffuseColorTransform[3] = {1,0,0};
        float modelSpecularColorTransform[3] = {1,1,1};
        float modelSpecularExponentTransform = 1.5;
        bool modelVIsualizationTypeFillPressedTransform;
        //Luces
        int createLightSelectedItem;
        PAG::lightTypes createLightType;
        float lightAmbientColor[3] = {0,0,0};
        float lightDiffuseColor[3] = {1,1,1};
        float lightSpecularColor[3] = {1,1,1};
        glm::vec3 lightPosition = glm::vec3(0,0,0);
        glm::vec3 lightDirection = glm::vec3(0,0,0);
        float lightGamma = 60.0f;
        float lightS = 1.0f;
        int currentLightIndex;
        bool createLightPressed;
        bool deleteLightPressed;
        float lightAmbientColorEdit[3] = {0,0,0};
        float lightDiffuseColorEdit[3] = {1,1,1};
        float lightSpecularColorEdit[3] = {1,1,1};
        glm::vec3 lightPositionEdit = glm::vec3(0,0,0);
        glm::vec3 lightDirectionEdit = glm::vec3(0,0,0);
        float lightGammaEdit = 60.0f;
        float lightSEdit = 1.0f;
        bool editLightPressed;
    public:
        virtual ~GUI();
        static GUI& getInstance();
        void guiInit(GLFWwindow* window);
        void shutDown();
        void newFrame();
        void render();
        void drawMessage(float posX,float posY,float fontScale,const char *title,const char *text);
        void drawColorWheel(float posX,float posY,float fontScale,float *col,const char *title,const char *text);
        const std::string &getShaderLoadInputText() const;
        void messageBufferAdd(std::string text);
        std::string getMessageBufferText();
        void drawCameraControls(float posX, float posY, float fontScale, const char *title);
        PAG::cameraMovementType getCameraMovement() const;
        float getCameraZoomValue() const;
        bool isDollyForwardPressed() const;
        bool isDollyBackwardPressed() const;
        bool isDollyLeftPressed() const;
        bool isDollyRightPressed() const;
        float getPanAngle() const;
        float getTiltAngle() const;
        bool isCraneUpPressed() const;
        bool isCraneDownPressed() const;
        float getLongitudeAngle() const;
        float getLatitudeAngle() const;
        void drawModelLoaderWindow(float posX, float posY, float fontScale, const char *title);
        const ImGui::FileBrowser &getFileBrowserWindow() const;
        void clearModelLoader();
        void drawModelTransformWindow(float posX, float posY, float fontScale, const char *title,
                                      std::vector<std::pair<PAG::Model, GLuint>> *modelList);
        bool isModelTransformApplyPressed() const;
        int getCurrentModelIndex() const;
        modelTransformType getModelTransform() const;
        const glm::vec3 &getModelTranslate() const;
        const glm::vec3 &getModelRotateAxis() const;
        float getModelRotateAngle() const;
        const glm::vec3 &getModelScale() const;
        bool isModelDeletePressed() const;
        void setCurrentModelIndex(int currentModelIndex);
        PAG::modelVisualizationTypes getModelVisualizationType() const;
        const float *getModelAmbientColor() const;
        const float *getModelAmbientColorTransform() const;
        const float *getModelDiffuseColor() const;
        const float *getModelSpecularColor() const;
        float getModelSpecularExponent() const;
        const float *getModelDiffuseColorTransform() const;
        const float *getModelSpecularColorTransform() const;
        float getModelSpecularExponentTransform() const;
        bool isModelVisualizationTypeFillPressedTransform() const;

        bool isModelMaterialApplyPressed() const;

        bool isCreateLightPressed() const;
        bool isDeleteLightPressed() const;

        lightTypes getCreateLightType() const;
        const float *getLightAmbientColor() const;
        const float *getLightDiffuseColor() const;
        const float *getLightSpecularColor() const;
        const glm::vec3 &getLightPosition() const;
        const glm::vec3 &getLightDirection() const;
        float getLightGamma() const;
        float getLightS() const;
        int getCurrentLightIndex() const;
        void setCurrentLightIndex(int currentLightIndex);

        const float *getLightAmbientColorEdit() const;

        const float *getLightDiffuseColorEdit() const;

        const float *getLightSpecularColorEdit() const;

        const glm::vec3 &getLightPositionEdit() const;

        const glm::vec3 &getLightDirectionEdit() const;

        float getLightGammaEdit() const;

        float getLightSEdit() const;

        bool isEditLightPressed() const;


        void drawMessageWindow(float posX, float posY, float fontScale);
        void drawControlWindow(float posX, float posY, float fontScale,
                               std::vector<std::pair<PAG::Model, GLuint>> *modelList,
                               std::vector<Light> *lightList, float *col,int lightCount);
    };
} // PAG

#endif //PAG_1_GUI_H

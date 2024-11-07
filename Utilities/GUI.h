//
// Created by DeJorgiK on 20/09/2024.
//

#ifndef PAG_1_GUI_H
#define PAG_1_GUI_H

#include <sstream>
#include "Camera.h"
#include "../imgui/imfilebrowser.h"
#include "Model.h"

namespace PAG {

    class GUI {
    private:
        static GUI* gui_instance;
        GUI();
        bool shaderLoadButtonPressed; //dice si el botón está pulsado o no
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
    public:
        virtual ~GUI();
        static GUI& getInstance();
        void guiInit(GLFWwindow* window);
        void shutDown();
        void newFrame();
        void render();
        void drawMessage(float posX,float posY,float fontScale,const char *title,const char *text);
        void drawColorWheel(float posX,float posY,float fontScale,float *col,const char *title,const char *text);
        void drawShaderLoadWindow(float posX,float posY,float fontScale,const char *title);
        bool isShaderLoadButtonPressed() const;
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
        void drawModelTransformWindow(float posX, float posY, float fontScale, const char *title);
        void drawModelTransformWindow(float posX, float posY, float fontScale, const char *title,
                                      std::vector<std::pair<PAG::Model, GLuint>> *modelList);
        bool isModelTransformApplyPressed() const;

        int getCurrentModelIndex() const;

        int getModelTransformSelectedItem() const;

        modelTransformType getModelTransform() const;

        const glm::vec3 &getModelTranslate() const;

        const glm::vec3 &getModelRotateAxis() const;

        float getModelRotateAngle() const;

        const glm::vec3 &getModelScale() const;
    };
} // PAG

#endif //PAG_1_GUI_H

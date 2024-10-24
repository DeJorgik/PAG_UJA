//
// Created by DeJorgiK on 20/09/2024.
//

#ifndef PAG_1_GUI_H
#define PAG_1_GUI_H

#include <GLFW/glfw3.h>
#include <sstream>
#include "Camera.h"

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
    };
} // PAG

#endif //PAG_1_GUI_H

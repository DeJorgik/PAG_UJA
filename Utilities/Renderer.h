#ifndef PAG_1_RENDERER_H
#define PAG_1_RENDERER_H

#include <iostream>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Camera.h"
#include "GUI.h"
#include "Model.h"
#include <glm/glm.hpp>

//Clase que encapsula el dibujado de la pantalla
//todas las funciones de opengl están aquí

//Sigue patrón singleton

namespace PAG {

    class Renderer {
    private:
        static Renderer* rederer_instance; //Instancia del Singleton
        Renderer(); //Constructor
        glm::vec3 bg_color; //Color de fondo 0 R, 1 G, 2 B, 3Alpha
        //PRÁCTICA 4, ShaderProgram
        ShaderProgram* shaderProgram;
        //PRÁCTICA 5, camara
        Camera* camera;
        int viewportWidth;
        int viewportHeight;
        //PRÁCTICA 6, lista con los modelos de la escena
        std::vector<PAG::Model> modelList;
    public:
        virtual ~Renderer ();
        static Renderer& getInstance ();
        void rendererInit();
        void windowRefresh();
        void viewportResize(int width, int height);
        void updateBgColor();
        float getBgColor(int color_id);
        glm::vec3* getBgColor();
        void setBgColor(int color_id,float value);
        void addBgColor(int color_id,double value);
        void createShaderProgram(std::string shaderProgramName);
        void createScene();
        bool operator==(const Renderer &rhs) const;
        bool operator!=(const Renderer &rhs) const;
        void getViewportSizes(int width, int height);
        void setUniformMVP();

        Camera *getCamera() const;

        void processUiCameraMovement(PAG::cameraMovementType movementType, float cameraZoomValue, float panAngle,
                                     float tiltAngle,
                                     bool dollyForward, bool dollyBackward, bool dollyLeft, bool dollyRight, bool craneUp,
                                     bool craneDown, float orbitLongitude, float orbitLatitude);
        void processMouseCameraMovement(double diffX, double diffY);
    };

} // PAG

#endif //PAG_1_RENDERER_H

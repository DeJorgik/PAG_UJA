//
// Created by DeJorgiK on 20/09/2024.
//

#ifndef PAG_1_GUI_H
#define PAG_1_GUI_H

#include <GLFW/glfw3.h>

namespace PAG {

    class GUI {
    private:
        static GUI* gui_instance;
        GUI();
        bool shaderLoadButtonPressed; //dice si el botón está pulsado o no
        std::string shaderLoadInputText; //Texto con el nombre del shader
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
    };
} // PAG

#endif //PAG_1_GUI_H

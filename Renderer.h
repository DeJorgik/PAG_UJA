#ifndef PAG_1_RENDERER_H
#define PAG_1_RENDERER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Clase que encapsula el dibujado de la pantalla
//todas las funciones de opengl están aquí

//Sigue patrón singleton

namespace PAG {

    class Renderer {

    private:
        static Renderer* rederer_instance; //Instancia del Singleton
        Renderer(); //Constructor
        GLfloat bg_color[]; //Color de fondo 0 R, 1 G, 2 B, 3Alpha
    public:
        virtual ~Renderer ();
        static Renderer& getInstance ();

        void rendererInit();

        void windowRefresh();
        void viewportResize(int width, int height);
        void updateBgColor();

        float getBgColor(int color_id);
        void setBgColor(int color_id,float value);
    };

} // PAG

#endif //PAG_1_RENDERER_H

#ifndef PAG_1_RENDERER_H
#define PAG_1_RENDERER_H

#include <iostream>
#include <glad/glad.h>
#include "ShaderProgram.h"

//Clase que encapsula el dibujado de la pantalla
//todas las funciones de opengl están aquí

//Sigue patrón singleton

namespace PAG {

    class Renderer {
    private:
        static Renderer* rederer_instance; //Instancia del Singleton
        Renderer(); //Constructor
        float bg_color[3]; //Color de fondo 0 R, 1 G, 2 B, 3Alpha
        //PRACTICA 3, IDENTIFICADORES
        GLuint idVAO = 0; //Identificador del vertex array object
        GLuint idVBO_pos = 0; //Identificador del vertex buffer object
        GLuint idVBO_color = 0;
        GLuint idVBO = 0;
        GLuint idIBO = 0; //Identificador del index buffer object
        //PRÁCTICA 4, ShaderProgram
        ShaderProgram* shaderProgram;
    public:
        virtual ~Renderer ();
        static Renderer& getInstance ();
        void rendererInit();
        void windowRefresh();
        void viewportResize(int width, int height);
        void updateBgColor();
        float getBgColor(int color_id);
        GLfloat* getBgColor();
        void setBgColor(int color_id,float value);
        void addBgColor(int color_id,double value);
        void createShaderProgram(std::string shaderProgramName);
        void createModel();
    };

} // PAG

#endif //PAG_1_RENDERER_H

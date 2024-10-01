#ifndef PAG_1_RENDERER_H
#define PAG_1_RENDERER_H

#include <iostream>
#include <glad/glad.h>

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
        GLuint idVs = 0; //Identificador vertex shader
        GLuint idFs = 0; //Identificador fragment shader
        GLuint idSP = 0; //Identificador shader program
        GLuint idVAO = 0; //Identificador del vertex array object
        GLuint idVBO = 0; //Identificador del vertex buffer object
        GLuint idIBO = 0; //Identificador del index buffer object

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

        //PRÁCICA 3:
        void createShaderProgram();
        void createModel();
    private:
        void shaderCompileErrorExceptionLaucher(GLuint shaderId);
        void shaderProgramCompileErrorExceptionLauncher(GLuint shaderProgramId);
    };

} // PAG

#endif //PAG_1_RENDERER_H

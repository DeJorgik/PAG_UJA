//
// Created by DeJorgiK on 04/10/2024.
//

#ifndef PAG_1_SHADERPROGRAM_H
#define PAG_1_SHADERPROGRAM_H

#include <iostream>
#include <glad/glad.h>
#include "ShaderObject.h"

//PRÁCTICA 4: práctica que encapsula las funciones relacionadas con los shaders
namespace PAG{
    class ShaderProgram {
    private:
        GLuint idSP; //Identificador shader program
        ShaderObject* vertexShader;
        ShaderObject* fragmentShader;
    public:
        ShaderProgram();
        ~ShaderProgram();
        //PRÁCICA 3:
        void createShaderProgram(std::string shaderProgramName);
        GLuint getIdSp() const;
    private:
        void shaderProgramCompileErrorExceptionLauncher(GLuint shaderProgramId);
    };
}

#endif //PAG_1_SHADERPROGRAM_H

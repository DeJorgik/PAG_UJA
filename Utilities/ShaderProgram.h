//
// Created by DeJorgiK on 04/10/2024.
//

#ifndef PAG_1_SHADERPROGRAM_H
#define PAG_1_SHADERPROGRAM_H

#include <iostream>
#include <glad/glad.h>
#include "ShaderObject.h"

namespace PAG{
    class ShaderProgram {
    private:
        std::string name; //nombre del programa
        GLuint idSP; //Identificador shader program
        ShaderObject* vertexShader;
        ShaderObject* fragmentShader;
    public:
        ShaderProgram();
        ~ShaderProgram();

        void createShaderProgram(std::string shaderProgramName);
        GLuint getIdSp() const;

        const std::string &getName() const;
        ShaderObject *getVertexShader() const;
        ShaderObject *getFragmentShader() const;

        void deleteShaderProgram();
    private:
        void shaderProgramCompileErrorExceptionLauncher(GLuint shaderProgramId);
    };
}

#endif //PAG_1_SHADERPROGRAM_H

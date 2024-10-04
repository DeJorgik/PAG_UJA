//
// Created by DeJorgiK on 04/10/2024.
//

#ifndef PAG_1_SHADEROBJECT_H
#define PAG_1_SHADEROBJECT_H

#include <iostream>
#include <glad/glad.h>
#include "ShaderObject.h"

namespace PAG{
    enum struct shaderType{VERTEX,FRAGMENT};
    class ShaderObject {
    private:
        GLuint id; //id del shader
        shaderType type; //tipo: si es de vertex o de fragment
    public:
        ShaderObject(shaderType _type);
        ~ShaderObject();
        void createShader(std::string shaderName);
        GLuint getId() const;

    private:
        std::string loadShader(std::string shaderName);
        void shaderCompileErrorExceptionLaucher(GLuint shaderId);
    };
}


#endif //PAG_1_SHADEROBJECT_H

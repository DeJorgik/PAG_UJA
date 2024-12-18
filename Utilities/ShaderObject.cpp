//
// Created by DeJorgiK on 04/10/2024.
//

#include <fstream>
#include <sstream>
#include "ShaderObject.h"
namespace PAG{

    ShaderObject::ShaderObject(shaderType _type) {
        id = 0;
        type = _type;
    }

    ShaderObject::~ShaderObject() {
        //if (id!=0){glDeleteShader(id);}
    }

    /**
     * Crea y compila el shader
     * @param shaderName
     */
    void ShaderObject::createShader(std::string shaderName) {
        std::string shader;
        //Diferencias entre tipos de shaders
        if(type == shaderType::VERTEX){
            shader = loadShader(shaderName+"-vs");
            id = glCreateShader(GL_VERTEX_SHADER);
            if (id==0){
                throw std::runtime_error("ERROR: Vertex Shader was not created.");
            }
        } else if(type == shaderType::FRAGMENT){
            shader = loadShader(shaderName+"-fs");
            id = glCreateShader(GL_FRAGMENT_SHADER);
            if (id==0){
                throw std::runtime_error("ERROR: Fragment Shader was not created.");
            }
        }
        //Parte común
        const GLchar* shaderCode = shader.c_str ();
        glShaderSource ( id, 1, &shaderCode, nullptr );
        glCompileShader ( id);
        //Comprobación de errores al compilar
        shaderCompileErrorExceptionLaucher(id);
    }

    /**
     * Carga el shader
     * @param shaderName
     * @return
     */
    std::string ShaderObject::loadShader(std::string shaderName) {
        std::ifstream shaderFile;
        shaderFile.open ("../Shaders/"+shaderName+".glsl");//no hace falta poner el formato
        if (!shaderFile.is_open()){ //excepción en caso de error
            throw std::runtime_error("ERROR: ../Shaders/"+shaderName+".glsl cannot be loaded.");
        }
        std::stringstream streamShader;
        streamShader << shaderFile.rdbuf();
        std::string shaderCode = streamShader.str();
        shaderFile.close();
        return shaderCode;
    }

    /**
     * Lanza excepciones si hay errores al compilar
     * @param shaderId
     */
    void ShaderObject::shaderCompileErrorExceptionLaucher(GLuint shaderId) {
        GLint shaderCompileResult = 0;
        glGetShaderiv( shaderId ,GL_COMPILE_STATUS,&shaderCompileResult);
        if(shaderCompileResult==GL_FALSE){
            std::string errorMsg;
            GLint msgSize = 0;
            glGetShaderiv(shaderId,GL_INFO_LOG_LENGTH,&msgSize);
            if(msgSize>0){
                auto* errorMsgFormatted = new GLchar[msgSize];
                GLint writtenData=0;
                glGetShaderInfoLog(shaderId,msgSize,&writtenData,errorMsgFormatted);
                errorMsg.assign(errorMsgFormatted);
                delete[] errorMsgFormatted;
                errorMsgFormatted = nullptr;
            }
            throw std::runtime_error(errorMsg);
        }
    }

    GLuint ShaderObject::getId() const {
        return id;
    }
}
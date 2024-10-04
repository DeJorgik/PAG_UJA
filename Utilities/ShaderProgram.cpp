//
// Created by DeJorgiK on 04/10/2024.
//

#include "ShaderProgram.h"

namespace PAG {

    ShaderProgram::ShaderProgram() {
        idSP=0;
        vertexShader=new ShaderObject(shaderType::VERTEX);
        fragmentShader= new ShaderObject(shaderType::FRAGMENT);
    }

    //Destruir los shaders que cuelgan
    ShaderProgram::~ShaderProgram() {
        if (idSP!=0){glDeleteShader(idSP);}
        delete &vertexShader;
        delete &fragmentShader;
    }

    void ShaderProgram::createShaderProgram(std::string shaderProgramName) {
        //Cargar shaders

        vertexShader->createShader(shaderProgramName);
        fragmentShader->createShader(shaderProgramName);

        //Shader program
        idSP = glCreateProgram ();
        if (idSP==0){
            throw std::runtime_error("ERROR: Shader Program was not created.");
        }
        glAttachShader ( idSP, vertexShader->getId() );
        glAttachShader ( idSP, fragmentShader->getId() );
        glLinkProgram ( idSP );
        shaderProgramCompileErrorExceptionLauncher(idSP);
    }

    void ShaderProgram::shaderProgramCompileErrorExceptionLauncher(GLuint shaderProgramId) {
        GLint shaderProgramLinkResult;
        glGetShaderiv( shaderProgramId ,GL_LINK_STATUS,&shaderProgramLinkResult);
        if(shaderProgramLinkResult==GL_FALSE){
            std::string errorMsg;
            GLint msgSize = 0;
            glGetShaderiv(shaderProgramId,GL_INFO_LOG_LENGTH,&msgSize);
            if(msgSize>0){
                auto* errorMsgFormatted = new GLchar[msgSize];
                GLint writtenData=0;
                glGetShaderInfoLog(shaderProgramId,msgSize,&writtenData,errorMsgFormatted);
                errorMsg.assign(errorMsgFormatted);
                delete[] errorMsgFormatted;
                errorMsgFormatted = nullptr;
            }
            throw std::runtime_error(errorMsg);
        }
    }

    GLuint ShaderProgram::getIdSp() const {
        return idSP;
    }
}
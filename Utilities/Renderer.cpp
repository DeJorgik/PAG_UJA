#include "Renderer.h"
#include <stdexcept>

namespace PAG {

    PAG::Renderer* PAG::Renderer::rederer_instance = nullptr;

    Renderer::Renderer() {
        //inicializa el fondo a gris
        bg_color[0] = 0.5f;
        bg_color[1] = 0.5f;
        bg_color[2] = 0.5f;
    }

    Renderer::~Renderer() {
        //Borrar shaders
        if (idVs!=0){glDeleteShader(idVs);}
        if (idFs!=0){glDeleteShader(idFs);}
        if (idSP!=0){glDeleteShader(idSP);}
        if (idVAO!=0){glDeleteShader(idVAO);}
        if (idVBO!=0){glDeleteShader(idVBO);}
        if (idIBO!=0){glDeleteShader(idIBO);}
    }

    Renderer &Renderer::getInstance() {
        if (!rederer_instance){
            rederer_instance = new Renderer;
        }

        return *rederer_instance;
    }

    void Renderer::rendererInit() {
        glEnable(GL_DEPTH_TEST);
        glEnable (GL_MULTISAMPLE);
        updateBgColor();
    }

    void Renderer::windowRefresh() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glUseProgram(idSP); //usar el shader program creado
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,nullptr); //dibujar elementos
    }

    void Renderer::viewportResize(int width, int height) {
        glViewport(0,0,width,height);
    }

    void Renderer::updateBgColor() {
        glClearColor(bg_color[0],bg_color[1],bg_color[2],1.0f);
    }

    float Renderer::getBgColor(int color_id) {
        return bg_color[color_id];
    }

    GLfloat *Renderer::getBgColor() {
        return bg_color;
    }

    void Renderer::setBgColor(int color_id, float value) {
        bg_color[color_id] = value;
        updateBgColor();
    }

    void Renderer::addBgColor(int color_id, double increment) {

        //Acotando (mas facil)
        bg_color[color_id]+=increment;

        if(bg_color[color_id]<0){
            bg_color[color_id] = 0.0f;
        } else if (bg_color[color_id]>1){
            bg_color[color_id] = 1.0f;
        }
        updateBgColor();
    }

    void Renderer::createShaderProgram() {
        //Código de los shaders
        std::string vertexShader =
                "#version 410\n"
                "layout (location = 0) in vec3 posicion;\n"
                "void main ()\n"
                "{ gl_Position = vec4 ( posicion, 1 );\n"
                "}\n";
        std::string fragmentShader =
                "#version 410\n"
                "out vec4 colorFragmento;\n"
                "void main ()\n"
                "{ colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
                "}\n";

        //Vertex Shader
        idVs = glCreateShader ( GL_VERTEX_SHADER );
        //Comprobación de errores al crear
        if (idVs==0){
            throw std::runtime_error("ERROR: Vertex Shader was not created.");
        }
        const GLchar* vertexShaderCode = vertexShader.c_str ();
        glShaderSource ( idVs, 1, &vertexShaderCode, nullptr );
        glCompileShader ( idVs);
        //Comprobación de errores al compilar
        shaderCompileErrorExceptionLaucher(idVs);

        //Fragment Shader
        idFs = glCreateShader ( GL_FRAGMENT_SHADER );
        if (idFs==0){
            throw std::runtime_error("ERROR: Fragment Shader was not created.");
        }
        const GLchar* fragmentShaderCode = fragmentShader.c_str ();
        glShaderSource ( idFs, 1, &fragmentShaderCode, nullptr );
        glCompileShader ( idFs );
        shaderCompileErrorExceptionLaucher(idFs);

        //Shader program
        idSP = glCreateProgram ();
        if (idSP==0){
            throw std::runtime_error("ERROR: Shader Program was not created.");
        }
        glAttachShader ( idSP, idVs );
        glAttachShader ( idSP, idFs );
        glLinkProgram ( idSP );
        shaderProgramCompileErrorExceptionLauncher(idSP);
    }

    void Renderer::createModel() {
        //Geometría y topología
        GLfloat vertices[] = {-.5,-.5,0,
                              .5,-.5,0,
                              .0,.5,0};
        GLuint indices[] = {0,1,2};

        //Generar VAO
        glGenVertexArrays (1,&idVAO);
        glBindVertexArray(idVAO);

        //Generar VBO (en este caso VBO de posicion)
        glGenBuffers(1,&idVBO);
        glBindBuffer(GL_ARRAY_BUFFER,idVBO);
        glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),vertices,GL_STATIC_DRAW); //OJO: se pone 9 porque el tamaño del vector de vertices es 9
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr); //Solo para vbo

        //Generar IBO
        glEnableVertexAttribArray(0);
        glGenBuffers(1,&idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(GLfloat),indices,GL_STATIC_DRAW); //OJO: se pone 9 porque el tamaño del vector de vertices es 9
    }

    /**
     * Función que lanza excepciones cuando hay errores en la compilación de los shaders
     */
    void Renderer::shaderCompileErrorExceptionLaucher(GLuint shaderId) {
        GLint vertexShaderCompileResult;
        glGetShaderiv( shaderId ,GL_COMPILE_STATUS,&vertexShaderCompileResult);
        if(vertexShaderCompileResult==GL_FALSE){
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

    /**
     * lo mismo pero cuando hay errores cuando se enlaza el programa
     */
    void Renderer::shaderProgramCompileErrorExceptionLauncher(GLuint shaderProgramId) {
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
} // PAG
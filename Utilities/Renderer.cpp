#include "Renderer.h"
#include "GUI.h"
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace PAG {

    PAG::Renderer* PAG::Renderer::rederer_instance = nullptr;

    Renderer::Renderer() {
        //inicializa el fondo a gris
        bg_color[0] = 0.5f;
        bg_color[1] = 0.5f;
        bg_color[2] = 0.5f;
    }

    Renderer::~Renderer() {
        if (idVBO_pos!=0){glDeleteShader(idVBO_pos);}
        if (idVBO_color!=0){glDeleteShader(idVBO_color);}
        if (idIBO!=0){glDeleteShader(idIBO);}
        delete &shaderProgram; //destruir shader program
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
        //Crear nuevo shader si le han dado a load
        if(GUI::getInstance().isShaderLoadButtonPressed()){
            if(GUI::getInstance().getShaderLoadInputText()!=""){
                delete shaderProgram; //borrar anterior
                shaderProgram = nullptr;
                try{
                    createShaderProgram(GUI::getInstance().getShaderLoadInputText());
                }catch (const std::exception& e){ //capturar excepción en caso de error
                    GUI::getInstance().messageBufferAdd(e.what());
                }
            }
        }
        glUseProgram(shaderProgram->getIdSp()); //usar el shader program creado
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

    void Renderer::createModel() {
        //Geometría y topología
        GLfloat vertices[] = {-.5,-.5,0,
                              .5,-.5,0,
                              .0,.5,0};
        GLuint indices[] = {0,1,2};
        GLfloat colors[] = {1,0,0,
                            0,1,0,
                            0,0,1};
        GLfloat verticesAndColors[] = {-.5,-.5,0,
                                       1,0,0,
                                       .5,-.5,0,
                                       0,1,0,
                                       .0,.5,0,
                                       0,0,1};

        //Generar VAO
        glGenVertexArrays (1,&idVAO);
        glBindVertexArray(idVAO);

        //Generar VBO)
        //Versión no entrelazada

        //VBO posición
        glGenBuffers(1,&idVBO_pos);
        glBindBuffer(GL_ARRAY_BUFFER,idVBO_pos);
        glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),vertices,GL_STATIC_DRAW); //OJO: se pone 9 porque el tamaño del vector de vertices es 9
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr); //Solo para vbo
        glEnableVertexAttribArray(0);

        //VBO color
        glGenBuffers(1,&idVBO_color);
        glBindBuffer(GL_ARRAY_BUFFER,idVBO_color);
        glBufferData(GL_ARRAY_BUFFER,9*sizeof(GLfloat),colors,GL_STATIC_DRAW);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr);
        glEnableVertexAttribArray(1);


        //Versión entrelazada
        /*
        glGenBuffers(1,&idVBO);
        glBindBuffer(GL_ARRAY_BUFFER,idVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(verticesAndColors),verticesAndColors,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(void*)0);
        glEnableVertexAttribArray(0); // atributo posición
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1); // atributo color
*/

        //Generar IBO
        glGenBuffers(1,&idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(GLfloat),indices,GL_STATIC_DRAW); //OJO: se pone 9 porque el tamaño del vector de vertices es 9
    }

    /**
     * Crea un shader program
     * @param shaderProgramName
     */
    void Renderer::createShaderProgram(std::string shaderProgramName) {
        //delete &shaderProgram; //Destruye el anterior
        shaderProgram = new ShaderProgram(); //crea uno nuevo
        shaderProgram->createShaderProgram(shaderProgramName);
    }

} // PAG
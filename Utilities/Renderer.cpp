#include "Renderer.h"
#include "GUI.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp> // value_ptr

namespace PAG {

    PAG::Renderer* PAG::Renderer::rederer_instance = nullptr;

    Renderer::Renderer() {
        //inicializa el fondo a gris
        bg_color.x = 0.5f;
        bg_color.y = 0.5f;
        bg_color.z = 0.5f;
        //Inicializa la camara
        //TODO poner bien
        camera = new Camera(glm::vec3(5,5,5),glm::vec3(0,0,0),glm::vec3(0,1,0),
                            60.f,0.5f,100.f,viewportWidth/viewportHeight,glm::vec3(0,1,0));
    }

    Renderer::~Renderer() {
        if (idVBO_pos!=0){glDeleteShader(idVBO_pos);}
        if (idVBO_color!=0){glDeleteShader(idVBO_color);}
        if (idIBO!=0){glDeleteShader(idIBO);}
        delete &shaderProgram; //destruir shader program
        delete &camera;
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
            } else{
                GUI::getInstance().messageBufferAdd("ERROR: No shader selected.");
            }
        }

        glUseProgram(shaderProgram->getIdSp()); //usar el shader program creado
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);
        setUniformMVP(); //aplicar cámara
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

    glm::vec3* Renderer::getBgColor() {
        return &bg_color;
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

    bool Renderer::operator==(const Renderer &rhs) const {
        return camera == rhs.camera;
    }

    bool Renderer::operator!=(const Renderer &rhs) const {
        return !(rhs == *this);
    }

    /**
     * Función para capturar el tamaño del viewport
     * @param width
     * @param height
     */
    void Renderer::getViewportSizes(int width, int height) {
        viewportWidth = width;
        viewportHeight = height;
        PAG::GUI::getInstance().messageBufferAdd("Viewport dimensions updated. Width: "+std::to_string(width)+" Height: "+std::to_string(height));
        camera->updateAspectRatio(width,height);//actualizar aspect ratio de la camara;
    }

    void Renderer::setUniformMVP()
    {
        GLint mvpLoc = glGetUniformLocation(shaderProgram->getIdSp(), "mModelViewProj");
        glm::mat4 p = camera->calculateProjectionMatrix();
                /*
        glm::mat4 ViewTranslate = glm::translate(
                glm::mat4(1.0f), Translate);
        glm::mat4 ViewRotateX = glm::rotate(
                ViewTranslate, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
        glm::mat4 View = glm::rotate(ViewRotateX,
                                     Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 Model = glm::scale(
                glm::mat4(1.0f), glm::vec3(0.5f));*/
        glm::mat4 v = camera->calculateViewMatrix();
        glm::mat4 mvp = p*v /* * Model*/;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    }

    Camera *Renderer::getCamera() const {
        return camera;
    }

    /**
     * Función que toma los parámetros de movimiento y llama a la cámara para que los efecue cuando sea necesario
     * @param movementType
     * @param cameraZoomValue
     * @param panAngle
     * @param tiltAngle
     * @param dollyForward
     * @param dollyBackward
     * @param dollyLeft
     * @param dollyRight
     * @param craneUp
     * @param craneDown
     * @param orbitLongitude
     * @param orbitLatitude
     */
    void Renderer::processUiCameraMovement(PAG::cameraMovementType movementType,
                                           float cameraZoomValue,
                                           float panAngle,
                                           float tiltAngle,
                                           bool dollyForward,
                                           bool dollyBackward,
                                           bool dollyLeft,
                                           bool dollyRight,
                                           bool craneUp,
                                           bool craneDown,
                                           float orbitLongitude,
                                           float orbitLatitude){
        switch (movementType) {
            case PAG::cameraMovementType::ZOOM:
                camera->updateZoom(cameraZoomValue);
                break;
            case PAG::cameraMovementType::PAN:
                camera->panMovement(panAngle);
                break;
            case PAG::cameraMovementType::TILT:
                camera->tiltMovement(tiltAngle);
                break;
            case PAG::cameraMovementType::DOLLY:
                //DUDA DOLLY: no deberia moverse en los ejes de la cámara??
                //Se mueve sobre los ejes de la escena pero entonces hacia delante no es hacia donde mira la cámara
                if (dollyForward){
                    camera->dollyCraneMovement(glm::vec3(0,0,-0.1));
                }
                if (dollyBackward){
                    camera->dollyCraneMovement(glm::vec3(0,0,0.1));
                }
                if (dollyLeft){
                    camera->dollyCraneMovement(glm::vec3(0.1,0,0));
                }
                if (dollyRight){
                    camera->dollyCraneMovement(glm::vec3(-0.1,0,0));
                }
                break;
            case PAG::cameraMovementType::CRANE:
                if (craneUp){
                    camera->dollyCraneMovement(glm::vec3(0,0.1,0));
                }
                if (craneDown){
                    PAG::Renderer::getInstance().getCamera()->dollyCraneMovement(glm::vec3(0,-0.1,0));
                }
                break;
            case PAG::cameraMovementType::ORBIT:
                camera->orbitMovement(orbitLongitude,
                                      orbitLatitude);
                break;
        }
    }

    void Renderer::processMouseCameraMovement(double diffX, double diffY) {
        //Movimiento en X
        if(diffX>0){
            camera->panMovement(0.5);
        } else if(diffX<0){
            camera->panMovement(-0.5);
        } else {
            camera->panMovement(0);
        }

        //Movimiento en Y
        if(diffY>0){
            camera->tiltMovement(-0.5);
        } else if(diffY<0){
            camera->tiltMovement(0.5);
        } else {
            camera->tiltMovement(0);
        }
    }


} // PAG
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
        modelList = new std::vector<std::pair<PAG::Model,GLuint>>();
    }

    Renderer::~Renderer() {
        //Destruir modelos
        for (auto &model:*modelList) {
            delete &model;
        }
        //delete &shaderProgram; //destruir shader program
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

        //Recorre lista de modelos, usando el par modelo/id
        //first es model, second es el int del shader program
        if(!modelList->empty()){
            for (auto &model:*modelList) {
                drawModel(model);
            }
        }
    }

    /**
     * Función para dibujar el modelo con su shaderprogram correspondiente
     */
    void Renderer::drawModel(std::pair<PAG::Model,GLuint> model){
        //Dibujar los modelos de la lista
        glBindVertexArray(model.first.getIdVao());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.first.getIdIbo());
        glUseProgram(model.second); //usar el shader program del modelo
        setUniformMVP(model.first,model.second); //aplicar cámara
        glDrawElements(GL_TRIANGLES, model.first.getIndices()->size(), GL_UNSIGNED_INT,
                       nullptr); //dibujar elementos
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

    /**Función que crea un modelo, crea el triangulo por defecto si no
     * se le pasa nombre
     */
    void Renderer::createModel(std::string modelName) {

        PAG::Model model = PAG::Model(modelName);

        //Generar VAO
        glGenVertexArrays (1, model.getIdVaoPointer());
        glBindVertexArray(model.getIdVao());

        //VBO posición
        glGenBuffers(1, model.getIdVboPosPointer());
        glBindBuffer(GL_ARRAY_BUFFER,model.getIdVboPos());
        glBufferData(GL_ARRAY_BUFFER,model.getVertices()->size()*sizeof(GLfloat),model.getVerticesArray(),GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr);
        glEnableVertexAttribArray(0);

        //VBO Normales
        glGenBuffers(1, model.getIdVboNormalsPointer());
        glBindBuffer(GL_ARRAY_BUFFER, model.getIdVboNormals());
        glBufferData(GL_ARRAY_BUFFER, model.getNormals()->size() * sizeof(GLfloat), model.getNormalsArray(), GL_STATIC_DRAW);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr);
        glEnableVertexAttribArray(1);

        //Generar IBO
        glGenBuffers(1, model.getIdIboPointer());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,model.getIdIbo());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,model.getIndices()->size()*sizeof(GLfloat),model.getIndicesArray(),GL_STATIC_DRAW);

        //Poner shader en el nombre
        model.setName(*model.getModelName()+"_"+shaderProgram->getName());

        //Cambiar nombre si hay un modelo con el mismo nombre
        for (auto &modelPair : *modelList) {
            if (*model.getModelName() == *modelPair.first.getModelName()){
                model.setName(*model.getModelName()+"_copy");
                break;
            }
        }

        //Guarda el shader actual con el modelo
        if(shaderProgram->getIdSp()!=0){ //guarda modelo cuando el shader program no es erróneo
            modelList->emplace_back(model,shaderProgram->getIdSp());
        }
    }

    /**
     * Función que borra un modelo de la lista
     * @param modelId
     */
    void Renderer::deleteModel(int modelId){
        if(!modelList->empty()){
            modelList->erase(modelList->begin()+modelId);
        }
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

    void Renderer::setUniformMVP(PAG::Model model,GLuint IdSp)
    {
        //Cuando el shaderprogram no sea 0
            GLint mvpLoc = glGetUniformLocation(IdSp, "mModelViewProj");
            glm::mat4 p = camera->calculateProjectionMatrix();
            glm::mat4 v = camera->calculateViewMatrix();
            glm::mat4 mvp = p*v*model.getModelMatrix();
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
                    camera->dollyCraneMovement(glm::vec3(0,0,0.2));
                }
                if (dollyBackward){
                    camera->dollyCraneMovement(glm::vec3(0,0,-0.2));
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

    /**
     * Función que aplica las transformaciones definidas en la interfaz al modelo correspondiente
     */
    void Renderer::processUiModelTransform(int modelId,
                                           PAG::modelTransformType modelTransformType,
                                           glm::vec3 modelTranslate,
                                           glm::vec3 modelRotateAxis,
                                           float modelRotateAngle,
                                           glm::vec3 modelScale){
        switch (modelTransformType) {
            case PAG::modelTransformType::TRANSLATE:
                modelList->at(modelId).first.modelTranslate(modelTranslate);
                break;
            case PAG::modelTransformType::ROTATE:
                modelList->at(modelId).first.modelRotate(modelRotateAxis,modelRotateAngle);
                break;
            case PAG::modelTransformType::SCALE:
                modelList->at(modelId).first.modelScale(modelScale);
                break;
            default:
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

    std::vector<std::pair<PAG::Model, GLuint>> *Renderer::getModelList() const {
        return modelList;
    }


} // PAG
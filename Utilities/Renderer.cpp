#include <glad/glad.h>
#include "Renderer.h"
#include "GUI.h"
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp> // value_ptr

namespace PAG {

    PAG::Renderer* PAG::Renderer::rederer_instance = nullptr;

    Renderer::Renderer() {
        //inicializa el fondo a gris
        bg_color.x = 0.5f;
        bg_color.y = 0.5f;
        bg_color.z = 0.5f;
        //Inicializa la camara
        camera = new Camera(glm::vec3(5,5,5),glm::vec3(0,0,0),glm::vec3(0,1,0),
                            60.f,0.5f,100.f,viewportWidth/viewportHeight,glm::vec3(0,1,0));
        modelList = new std::vector<std::pair<PAG::Model,PAG::ShaderProgram>>();
        lightList = new std::vector<PAG::Light>();
    }

    Renderer::~Renderer() {
        //Destruir modelos
        for (int i = 0; i<modelList->size() ;i++) {
            deleteModel(i);
        }
        //Destuir Cámaras
        delete camera;
    }

    Renderer &Renderer::getInstance() {
        if (!rederer_instance){
            rederer_instance = new Renderer;
        }
        return *rederer_instance;
    }

    void Renderer::rendererInit() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable (GL_MULTISAMPLE);
        glEnable(GL_BLEND); //activar blending
        updateBgColor();
    }

    /**
     * Bucle for enlazado: para cada luz para cada modelo -> rendering multipasada
     */
    void Renderer::windowRefresh() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        if(!lightList->empty()){
            for (int i = 0; i < lightList->size(); ++i) {
                if(!modelList->empty()){
                    for (auto &model:*modelList) {
                        //Recorre lista de modelos, usando el par modelo/id
                        //first es model, second es el int del shader program
                        drawModel(&model,i);
                    }
                }
            }
        }
    }

    /**
     * Función para dibujar el modelo con su shaderprogram correspondiente
     */
    void Renderer::drawModel(std::pair<PAG::Model,PAG::ShaderProgram> *modelPair, int lightId){
        //Activar para cada luz
        if(lightId==0){
            //primera luz
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } else if (lightId==1){
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        }

        glUseProgram(modelPair->second.getIdSp()); //usar el shader program del modelo

        //Lista de índices de subrutinas
        //La el primer indice es para la subrutina de color difuso y el segundo para el cálculo de las luces
        GLuint subroutineIndex[] = {0,0};
        int indexTexSubroutine = glGetSubroutineUniformLocation(modelPair->second.getIdSp(),GL_FRAGMENT_SHADER, "getDiffuseColorMethod");
        int indexLightSubroutine = glGetSubroutineUniformLocation(modelPair->second.getIdSp(),GL_FRAGMENT_SHADER, "getColorMethod");

        //Elegir subrutina de textura
        if(modelPair->first.getModelVisualizationType()==PAG::modelVisualizationTypes::TEXTURED){
            subroutineIndex[indexTexSubroutine] = glGetSubroutineIndex(modelPair->second.getIdSp(), GL_FRAGMENT_SHADER, "textureColor");
        } else {
            subroutineIndex[indexTexSubroutine] = glGetSubroutineIndex(modelPair->second.getIdSp(), GL_FRAGMENT_SHADER, "materialColor");
        }

        //Elegir subrutina de luz
        if(modelPair->first.getModelVisualizationType()!=PAG::modelVisualizationTypes::WIREFRAME){
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            //Dentro del modo de FILL hay que elegir una de las subrutinas segun el tipo de luz
            switch (lightList->at(lightId).getLightType()) {
                case PAG::lightTypes::AMBIENT:
                    subroutineIndex[indexLightSubroutine] = glGetSubroutineIndex(modelPair->second.getIdSp(), GL_FRAGMENT_SHADER, "ambientLight");
                    break;
                case PAG::lightTypes::DIRECTION:
                    subroutineIndex[indexLightSubroutine] = glGetSubroutineIndex(modelPair->second.getIdSp(), GL_FRAGMENT_SHADER, "directionLight");
                    break;
                case PAG::lightTypes::POINT:
                    subroutineIndex[indexLightSubroutine] = glGetSubroutineIndex(modelPair->second.getIdSp(), GL_FRAGMENT_SHADER, "pointLight");
                    break;
                case PAG::lightTypes::SPOT:
                    subroutineIndex[indexLightSubroutine] = glGetSubroutineIndex(modelPair->second.getIdSp(), GL_FRAGMENT_SHADER, "spotLight");
                    break;
                default:
                    break;
            }
        } else{
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            subroutineIndex[indexLightSubroutine] = glGetSubroutineIndex(modelPair->second.getIdSp(), GL_FRAGMENT_SHADER, "wireframe");
        }

        glUniformSubroutinesuiv ( GL_FRAGMENT_SHADER, 2, subroutineIndex );

        setUniformMVandMVP(&modelPair->first,modelPair->second.getIdSp()); //Enviar MV y MVP al shader
        setUniformLight(&lightList->at(lightId),modelPair->second.getIdSp()); //Enviar datos de la luz
        setUniformMaterial(modelPair->first.getMaterial(),modelPair->second.getIdSp());//Enviar datos del material

        //Texturas
        // Asignamos el muestreador del shader program a la unidad de textura 0
        GLint samplerPos = glGetUniformLocation ( modelPair->second.getIdSp(), "sampler" );
        glUniform1i ( samplerPos, 0 );

        //Activar unidad de textura si se necesita
        if(modelPair->first.getModelVisualizationType()==PAG::modelVisualizationTypes::TEXTURED){
            glActiveTexture ( GL_TEXTURE0 );
            glBindTexture ( GL_TEXTURE_2D, *modelPair->first.getIdTexture() );
        }

        //Dibujar los modelos de la lista
        glBindVertexArray(modelPair->first.getIdVao());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelPair->first.getIdIbo());
        glDrawElements(GL_TRIANGLES, modelPair->first.getIndices()->size(), GL_UNSIGNED_INT,
                       nullptr); //dibujar elementos
    }

    void Renderer::viewportResize(int width, int height) {
        glViewport(0,0,width,height);
    }

    void Renderer::updateBgColor() {
        glClearColor(bg_color[0],bg_color[1],bg_color[2],1.0f);
    }

    glm::vec3* Renderer::getBgColor() {
        return &bg_color;
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

    /**
     * Función que crea un modelo y su shader program asociado
     * Crea el triangulo por defecto si nose le pasa nombre de modelo
     */
    void Renderer::createModelPair(std::string shaderProgramName,
                               std::string modelName, std::string textureName,
                               modelVisualizationTypes modelVisualizationType,
                               const glm::vec3 &ambient, const glm::vec3 &diffuse,const glm::vec3 &specular,
                               GLfloat exponent) {

        //Crear Shader Program
        PAG::ShaderProgram shaderProgram = PAG::ShaderProgram();
        shaderProgram.createShaderProgram(shaderProgramName);

        PAG::Model model = PAG::Model(modelName,modelVisualizationType); //crear modelo
        model.createMaterial(ambient,diffuse,specular,exponent); //crear y aplicar material

        //Aplicar VAO
        model.createVaoOpenGL();

        //Poner shader en el nombre
        model.setName(*model.getModelName()+"_"+shaderProgram.getName());

        //Cambiar nombre si hay un modelo con el mismo nombre
        for (auto &modelPair : *modelList) {
            if (*model.getModelName() == *modelPair.first.getModelName()){
                model.setName(*model.getModelName()+"_copy");
            }
        }

        //Guarda el shader actual con el modelo
        if(shaderProgram.getIdSp()!=0){ //guarda modelo cuando el shader program no es erróneo
            modelList->emplace_back(model,shaderProgram);
        }

        //Aplicar textura
        if(modelVisualizationType==PAG::modelVisualizationTypes::TEXTURED){
            model.loadTexture(textureName); //cargar textura
        }
    }

    /**
     * Función que borra un modelo de la lista
     * @param modelId
     */
    void Renderer::deleteModel(int modelId){
        if(!modelList->empty()){
            //Destruir shader Objects y shader Program
            if(modelList->at(modelId).second.getIdSp()!=0){
                glDeleteShader(modelList->at(modelId).second.getVertexShader()->getId());
                glDeleteShader(modelList->at(modelId).second.getFragmentShader()->getId());
                glDeleteProgram(modelList->at(modelId).second.getIdSp());
            }
            modelList->at(modelId).second.deleteShaderProgram();

            //Destruir textura del modelo
            if(modelList->at(modelId).first.getIdTexture()!=0){
                glDeleteTextures(1,modelList->at(modelId).first.getIdTexture());}

                modelList->erase(modelList->begin()+modelId);
        }
    }

    void Renderer::createLight(PAG::lightTypes lightType,
                              glm::vec3 Ia,glm::vec3 Is,glm::vec3 Id,
                              glm::vec3 pos, glm::vec3 d,
                              float gamma, float s){

        //Crear luz por defecto
        PAG::Light light = Light();

        //cambiar tipo de luz
        switch (lightType) {
            case AMBIENT:
                light.setAmbientLight("ambient_"+std::to_string(lightList->size()+1),
                                      Ia);
                break;
            case DIRECTION:
                light.setDirectionLight("direction_"+std::to_string(lightList->size()+1),
                                        Id,Is,
                                        d);
                break;
            case POINT:
                light.setPointLight("point_"+std::to_string(lightList->size()+1),
                                    Id,Is,
                                    pos);
                break;
            case SPOT:
                light.setSpotLight("spot_"+std::to_string(lightList->size()+1),
                                    Id,Is,
                                    pos,d,
                                    gamma,s);
                break;
            default:
                break;
        }

        //Cambiar nombre si hay un modelo con el mismo nombre
        for (auto &_light : *lightList) {
            if (light.getLightName() == _light.getLightName()){
                light.setLightName(_light.getLightName()+"_copy");
            }
        }

        lightList->push_back(light);
    }

    void Renderer::deleteLight(int lightId){
        if(lightList->size()>1){
            lightList->erase(lightList->begin()+lightId);
        }
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

    /**
     * Ahora esta función también manda MV como uniform
     * @param model
     * @param IdSp
     */
    void Renderer::setUniformMVandMVP(PAG::Model *model,GLuint IdSp)
    {
        GLint mvLoc = glGetUniformLocation(IdSp, "mModelView");
        GLint mvpLoc = glGetUniformLocation(IdSp, "mModelViewProj");
        glm::mat4 v = camera->calculateViewMatrix();
        glm::mat4 p = camera->calculateProjectionMatrix();
        glm::mat4 mv = v*model->getModelMatrix();
        glm::mat4 mvp = p*v*model->getModelMatrix();
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    }

    /**
     * Función que pasa los colores ambient diffuse y specular como atributos uniform al shader program
     * @param material
     */
    void Renderer::setUniformMaterial(PAG::Material* material, GLuint IdSp){
        GLint ambientLoc = glGetUniformLocation(IdSp, "Ka");
        GLint diffuseLoc = glGetUniformLocation(IdSp, "Kd");
        GLint specularLoc = glGetUniformLocation(IdSp, "Ks");
        GLint exponentLoc = glGetUniformLocation(IdSp, "exponent");

        //Si no se utiliza el uniform, no lo enlaza, poniendo la localizazión en -1
        if(ambientLoc!=-1){
            float ambientColor[] = {material->getAmbient().x, material->getAmbient().y, material->getAmbient().z};
            glUniform3fv(ambientLoc, 1,ambientColor);
        }
        if(diffuseLoc!=-1){
            float diffuseColor[] = {material->getDiffuse().x, material->getDiffuse().y, material->getDiffuse().z};
            glUniform3fv(diffuseLoc, 1,diffuseColor);
        }
        if(specularLoc!=-1){
            float specularColor[] = {material->getSpecular().x, material->getSpecular().y, material->getSpecular().z};
            glUniform3fv(specularLoc, 1,specularColor);
        }
        if(exponentLoc!=-1){
            float exponent = material->getExponent();
            glUniform1fv(exponentLoc, 1,&exponent);
        }
    }

    /**
     * Lo mismo pero para la luz
     * @param light
     * @param IdSp
     */
    void Renderer::setUniformLight(PAG::Light* light,  GLuint IdSp){
        GLint ambientLoc = glGetUniformLocation(IdSp, "Ia");
        GLint diffuseLoc = glGetUniformLocation(IdSp, "Id");
        GLint specularLoc = glGetUniformLocation(IdSp, "Is");
        GLint positionLoc = glGetUniformLocation(IdSp, "lightPos");
        GLint directionLoc = glGetUniformLocation(IdSp, "lightDir");
        GLint spotAngleLoc = glGetUniformLocation(IdSp, "spotAngle");

        //Matriz de visión
        glm::mat4 v = camera->calculateViewMatrix();

        if(ambientLoc!=-1){
            float ambientColor[] = {light->getIa().x, light->getIa().y, light->getIa().z};
            glUniform3fv(ambientLoc, 1,ambientColor);
        }
        if(diffuseLoc!=-1){
            float diffuseColor[] = {light->getId().x, light->getId().y, light->getId().z};
            glUniform3fv(diffuseLoc, 1,diffuseColor);
        }
        if(specularLoc!=-1){
            float specularColor[] = {light->getIs().x, light->getIs().y, light->getIs().z};
            glUniform3fv(specularLoc, 1,specularColor);
        }
        if(positionLoc!=-1){
            glm::vec3 visionPosition = glm::vec3(v*glm::vec4(light->getPos(),1.0));//pasar a espacio de vision
            float position[] = {visionPosition.x, visionPosition.y, visionPosition.z};
            glUniform3fv(positionLoc, 1,position);
        }
        if(directionLoc!=-1){
            float direction[3];
            if(light->getLightType()==lightTypes::SPOT){
                glm::vec3 visionDirection = glm::vec3(v*glm::vec4(light->getD(),1.0));//pasar a espacio de vision
                direction[0]=visionDirection.x;
                direction[1]=visionDirection.y;
                direction[2]=visionDirection.z;
            } else {
                direction[0]=light->getD().x;
                direction[1]=light->getD().y;
                direction[2]=light->getD().z;
            }
            glUniform3fv(directionLoc, 1,direction);
        }
        if(spotAngleLoc!=-1){
            float angle = glm::radians(light->getGamma());
            glUniform1fv(spotAngleLoc, 1,&angle);
        }
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
                                           float orbitLatitude,
                                           bool reset){
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

        if (reset) {
            //Resetear la cámara
            camera->setCameraPos(glm::vec3(5,5,5));
            camera->setLookAtPoint(glm::vec3(0,0,0));
            camera->setFovY(60.f);
            camera->updateCoordinateSystem();
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

    void Renderer::processUiModelMaterial(int modelId,
                                          glm::vec3 modelAmbientTransform,
                                          glm::vec3 modelDiffuseTransform,
                                          glm::vec3 modelSpecularTransform,
                                          float modelExponentTransform,
                                          PAG::modelVisualizationTypes modelVisualizationTypes){
        //Cambiar material del modelo
        modelList->at(modelId).first.setModelVisualizationType(modelVisualizationTypes);
        modelList->at(modelId).first.getMaterial()->setAmbient(modelAmbientTransform);
        modelList->at(modelId).first.getMaterial()->setDiffuse(modelDiffuseTransform);
        modelList->at(modelId).first.getMaterial()->setSpecular(modelSpecularTransform);
        modelList->at(modelId).first.getMaterial()->setExponent(modelExponentTransform);
    }

    void Renderer::processUiLightEdit(int lightId,
                                      glm::vec3 Ia,glm::vec3 Is,glm::vec3 Id,
                                      glm::vec3 pos, glm::vec3 d,
                                      float gamma, float s){

        //Editar segun tipo de luz
        switch (lightList->at(lightId).getLightType()) {
            case AMBIENT:
                lightList->at(lightId).setAmbientLight(Ia);
                break;
            case DIRECTION:
                lightList->at(lightId).setDirectionLight(Id,Is,d);
                break;
            case POINT:
                lightList->at(lightId).setPointLight(Id,Is,pos);
                break;
            case SPOT:
                lightList->at(lightId).setSpotLight(Id,Is,pos,d,gamma,s);
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

    void Renderer::processKeyCameraMovement(glm::vec3 ammount) {
        camera->dollyCraneMovement(ammount);
    }

    void Renderer::processMouseCameraZoom(double yoffset) {
        double ammount=0;
        if(yoffset>0){
            ammount=-1;
        }else if(yoffset<0){
            ammount=1;
        }
        camera->addZoom(ammount);
    }

    std::vector<PAG::Light> *Renderer::getLightList() const {
        return lightList;
    }

    void Renderer::loadTexture(std::string textureName, int idModel) {
        modelList->at(idModel).first.loadTexture(textureName);
    }

    std::vector<std::pair<PAG::Model, PAG::ShaderProgram>> *Renderer::getModelList() const {
        return modelList;
    }

} // PAG
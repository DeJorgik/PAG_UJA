#ifndef PAG_1_RENDERER_H
#define PAG_1_RENDERER_H

#include <gl/gl.h>
#include <iostream>
#include "ShaderProgram.h"
#include "Camera.h"
#include "GUI.h"
#include "Model.h"
#include "Light.h"
#include <glm/glm.hpp>

//Clase que encapsula el dibujado de la pantalla
//todas las funciones de opengl están aquí

//Sigue patrón singleton

namespace PAG {

    class Renderer {
    private:
        static Renderer* rederer_instance; //Instancia del Singleton
        Renderer(); //Constructor
        glm::vec3 bg_color; //Color de fondo 0 R, 1 G, 2 B, 3Alpha
        Camera* camera;
        int viewportWidth = 1; //inicializado a 1 para que no de error en linux
        int viewportHeight = 1;
        std::vector<std::pair<PAG::Model,PAG::ShaderProgram>>* modelList;//lista con los pares Modelo/Shader Program asociado
        std::vector<PAG::Light>* lightList; //luces de la escena
    public:
        virtual ~Renderer ();
        static Renderer& getInstance ();
        void rendererInit();
        void windowRefresh();
        void viewportResize(int width, int height);
        void updateBgColor();
        glm::vec3* getBgColor();
        void addBgColor(int color_id,double value);
        bool operator==(const Renderer &rhs) const;
        bool operator!=(const Renderer &rhs) const;
        void getViewportSizes(int width, int height);

        Camera *getCamera() const;
        void processUiCameraMovement(PAG::cameraMovementType movementType, float cameraZoomValue, float panAngle,
                                     float tiltAngle,
                                     bool dollyForward, bool dollyBackward, bool dollyLeft, bool dollyRight, bool craneUp,
                                     bool craneDown, float orbitLongitude, float orbitLatitude,
                                     bool reset);
        void processMouseCameraMovement(double diffX, double diffY);
        void processMouseCameraZoom(double yoffset);
        void processKeyCameraMovement(glm::vec3 ammount);

        std::vector<std::pair<PAG::Model, PAG::ShaderProgram>> *getModelList() const;
        void createModelPair(std::string shaderProgramName, std::string modelName, std::string textureName,
                             std::string normalMapName, modelVisualizationTypes modelVisualizationType,
                             const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                             GLfloat exponent);

        void drawModel(std::pair<Model, ShaderProgram> *modelPair, int lightId);
        void deleteModel(int modelId);


        void
        createLight(lightTypes lightType, glm::vec3 Ia, glm::vec3 Is, glm::vec3 Id,
                    glm::vec3 pos, glm::vec3 d,
                    float gamma,
                    float s);

        std::vector<PAG::Light> *getLightList() const;
        void deleteLight(int lightId);

        void processUiModelTransform(int modelId, modelTransformType modelTransformType, glm::vec3 modelTranslate,
                                     glm::vec3 modelRotateAxis, float modelRotateAngle, glm::vec3 modelScale);
        void processUiLightEdit(int lightId, glm::vec3 Ia, glm::vec3 Is, glm::vec3 Id, glm::vec3 pos, glm::vec3 d,
                                float gamma,
                                float s);
        void processUiModelMaterial(int modelId, glm::vec3 modelAmbientTransform, glm::vec3 modelDiffuseTransform,
                                    glm::vec3 modelSpecularTransform, float modelExponentTransform,
                                    modelVisualizationTypes modelVisualizationTypes);

        void loadTexture(std::string textureName, int idModel);

        void setUniformMVandMVP(Model *model, GLuint IdSp);
        void setUniformMaterial(Material *material, GLuint IdSp);
        void setUniformLight(Light *light, GLuint IdSp);
    };

} // PAG

#endif //PAG_1_RENDERER_H

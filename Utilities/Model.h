//
// Created by DeJorgiK on 25/10/2024.
//

#ifndef PAG_1_MODEL_H
#define PAG_1_MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Material.h"

#include "../lodepng/lodepng.h"

namespace PAG {
    //Tipo de transformación del modelo
    enum modelTransformType{TRANSLATE,ROTATE,SCALE};

    //Tipo de visualización del modelo
    enum modelVisualizationTypes{WIREFRAME,FILL,TEXTURED,TEXTURED_AND_NORMAL};

    class Model {
    private:
        std::string name; //Nombre del modelo (se usa para el combo)
        modelVisualizationTypes modelVisualizationType; //Tipo de visualización
        int vertexCount;
        std::vector<GLfloat> *vertices;
        std::vector<GLuint> *indices;
        std::vector<GLfloat> *normals;
        std::vector<GLfloat> *textureCoordinates;
        std::vector<GLfloat> *tangents;
        std::vector<GLfloat> *bitangents;
        glm::mat4 modelMatrix;
        GLuint idVAO = 0;
        GLuint idVBO_pos = 0;
        GLuint idVBO_normals = 0;
        GLuint idVBO_textureCoordinates = 0;
        GLuint idVBO_tangents = 0;
        GLuint idVBO_bitangents = 0;
        GLuint idIBO = 0;
        Material* material;
        std::string textureName = "";
        std::string normalMapName = "";
        std::vector<unsigned char> texturePixels, normalMapPixels;
        unsigned textureWidth, textureHeight, normalMapWidth, normalMapHeight;
        GLuint *idTexture = nullptr;
        GLuint *idNormalMap = nullptr;
        void drawDefaultTriangle();
    public:

        Model(std::string filename, modelVisualizationTypes modelVisualizationType);
        virtual ~Model();

        std::vector<GLuint> *getIndices() const;
        GLuint getIdVao();
        GLuint getIdIbo();
        GLuint * getIdTexture() const;
        GLuint *getIdNormalMap() const;

        void setName(const std::string &name);
        std::string *getModelName();

        PAG::modelVisualizationTypes getModelVisualizationType() const;
        const glm::mat4 &getModelMatrix() const;

        void sceneProcess(const aiScene *assimpScene);
        void meshProcess(aiMesh *mesh);

        void modelTranslate(glm::vec3 transform);
        void modelRotate(glm::vec3 axis, float angle);
        void modelScale(glm::vec3 scale);
        void setModelVisualizationType(modelVisualizationTypes modelVisualizationType);

        void createMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, GLfloat exponent);
        Material *getMaterial() const;

        void loadTexture(std::string filename);

        void createTextureOpenGL();
        void createVaoOpenGL();

        void createNormalMapOpenGL();

        void loadNormalMap(std::string filename);
    };
} // PAG

#endif //PAG_1_MODEL_H

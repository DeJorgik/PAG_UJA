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
    enum modelVisualizationTypes{WIREFRAME,FILL,TEXTURED};

    class Model {
    private:
        std::string name; //Nombre del modelo (se usa para el combo)
        modelVisualizationTypes modelVisualizationType; //Tipo de visualización
        int vertexCount;
        std::vector<GLfloat> *vertices;
        std::vector<GLuint> *indices;
        std::vector<GLfloat> *normals;
        std::vector<GLfloat> *textureCoodrinates;
        glm::mat4 modelMatrix;
        //PAG::modelType modelType;
        GLuint idVAO = 0;
        GLuint idVBO_pos = 0;
        GLuint idVBO_normals = 0;
        GLuint idVBO_textureCoordinates = 0;
        GLuint idIBO = 0;
        //PRACTICA 7
        Material* material;
        //PRACTICA 9
        std::vector<unsigned char> texturePixels;
        unsigned textureWidth, textureHeight;
        GLuint *idTexture = nullptr;
        void drawDefaultTriangle();
    public:
        //Model(); //Crea el modelo del triángulo por defecto
        //Model(std::string filename, PAG::modelType modelType, int shaderProgramId); //Carga el modelo desde el nombre
        Model(std::string filename, modelVisualizationTypes modelVisualizationType);
        virtual ~Model();

        std::vector<GLfloat> *getVertices() const;
        std::vector<GLuint> *getIndices() const;
        std::vector<GLfloat> *getNormals() const;
        GLfloat* getVerticesArray();
        GLfloat* getNormalsArray();
        GLuint* getIndicesArray();

        GLuint getIdVao();
        GLuint *getIdVaoPointer();
        GLuint getIdVboPos();
        GLuint *getIdVboPosPointer();
        GLuint getIdVboNormals();
        GLuint *getIdVboNormalsPointer();
        GLuint getIdIbo();
        GLuint *getIdIboPointer();

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
        void setWireframe(bool isWireframe);

        void createMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, GLfloat exponent);
        Material *getMaterial() const;



        const std::vector<unsigned char> &getTexturePixels() const;
        void setTexturePixels(const std::vector<unsigned char> &texturePixels);
        unsigned int getTextureWidth() const;
        void setTextureWidth(unsigned int textureWidth);
        unsigned int getTextureHeight() const;
        void setTextureHeight(unsigned int textureHeight);

        void loadTexture(std::string filename);

        GLuint *getTextureCoordinatesArray();

        GLuint getIdVboTextureCoodrinates();

        GLuint *getIdVboTextureCoodrinatesPointer();

        std::vector<GLfloat> *getTextureCoodrinates() const;

        GLuint * getIdTexture() const;
    };
} // PAG

#endif //PAG_1_MODEL_H

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

namespace PAG {
    //Tipo de transformación del modelo
    enum modelTransformType{TRANSLATE,ROTATE,SCALE};

    //Tipo de visualización del modelo
    enum modelVisualizationTypes{WIREFRAME,FILL};

    class Model {
    private:
        Model(std::string filename, modelVisualizationTypes modelVisualizationType, Material material);

        std::string name; //Nombre del modelo (se usa para el combo)
        modelVisualizationTypes modelVisualizationType; //Tipo de visualización
        int vertexCount;
        std::vector<GLfloat> *vertices;
        std::vector<GLuint> *indices;
        std::vector<GLfloat> *normals;
        std::vector<GLfloat> *materialColors; //vector que contiene en orden los colores ambiente,difuso y especular del modelo, el último valor es el exponente
        //std::vector<GLfloat> *textureCoordinates;
        glm::mat4 modelMatrix;
        //PAG::modelType modelType;
        GLuint idVAO = 0;
        GLuint idVBO_pos = 0;
        GLuint idVBO_normals = 0;
        GLuint idVBO_materialColors = 0; //VBO ENTRELAZADO DE LOS COLORES DEL MATERIAL
        //GLuint idVBO_textureCoordinates = 0;
        GLuint idIBO = 0;
        //PRACTICA 7
        Material* material;
        void drawDefaultTriangle();
        void createDefaultMaterial(); //Ahora para la prueba
    public:
        //Model(); //Crea el modelo del triángulo por defecto
        //Model(std::string filename, PAG::modelType modelType, int shaderProgramId); //Carga el modelo desde el nombre
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
        GLuint getIdVboMaterialColors();
        GLuint *getIdVboMaterialColorsPointer();
        GLuint getIdIbo();
        GLuint *getIdIboPointer();
        const glm::mat4 &getModelMatrix() const;

        void sceneProcess(const aiScene *assimpScene);
        void meshProcess(aiMesh *mesh);
        Model(std::string filename);

        void setName(const std::string &name);

        std::string *getModelName();

        void modelTranslate(glm::vec3 transform);
        void modelRotate(glm::vec3 axis, float angle);
        void modelScale(glm::vec3 scale);

        void setModelVisualizationType(modelVisualizationTypes modelVisualizationType);

        PAG::modelVisualizationTypes getModelVisualizationType() const;

        void
        createMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, GLfloat exponent);

        GLfloat *getMaterialColorsArray();

        Material *getMaterial() const;

        Model(std::string filename, modelVisualizationTypes modelVisualizationType);
    };
} // PAG

#endif //PAG_1_MODEL_H

//
// Created by DeJorgiK on 25/10/2024.
//

#ifndef PAG_1_MODEL_H
#define PAG_1_MODEL_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>


#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


namespace PAG {
    //Tipo de modelo para que haga algo distinto según si estan coloreados, texturizados o sin nada.
    //enum modelType {BLANK,COLORED,TEXTURED};
    class Model {
    private:

        int vertexCount;
        std::vector<GLfloat> *vertices;
        std::vector<GLuint> *indices;
        std::vector<GLfloat> *normals;
        //std::vector<GLfloat> *textureCoordinates;
        glm::mat4 modelMatrix;
        //PAG::modelType modelType;
        GLuint idVAO = 0;
        GLuint idVBO_pos = 0;
        GLuint idVBO_normals = 0;
        //GLuint idVBO_textureCoordinates = 0;
        GLuint idIBO = 0;
        void drawDefaultTriangle();
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
        GLuint getIdIbo();
        GLuint *getIdIboPointer();
        GLuint getShaderProgramId() const;
        const glm::mat4 &getModelMatrix() const;
        //PAG::modelType getModelType() const;

        void sceneProcess(const aiScene *assimpScene);
        void meshProcess(aiMesh *mesh);
        Model(std::string filename);
    };
} // PAG

#endif //PAG_1_MODEL_H

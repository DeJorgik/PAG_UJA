//
// Created by DeJorgiK on 25/10/2024.
//

#ifndef PAG_1_MODEL_H
#define PAG_1_MODEL_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>

namespace PAG {
    //Tipo de modelo para que haga algo distinto según si estan coloreados, texturizados o sin nada.
    //enum modelType {BLANK,COLORED,TEXTURED};
    class Model {
    private:
        std::vector<GLfloat> *vertices;
        std::vector<GLfloat> *indices;
        std::vector<GLfloat> *normals;
        //std::vector<GLfloat> *textureCoordinates;
        glm::mat4 modelMatrix;
        //PAG::modelType modelType;
        GLuint shaderProgramId;
        GLuint idVAO = 0;
        GLuint idVBO_pos = 0;
        GLuint idVBO_normals = 0;
        //GLuint idVBO_textureCoordinates = 0;
        GLuint idIBO = 0;
        void drawDefaultTriangle();
    public:
        //Model(); //Crea el modelo del triángulo por defecto
        //Model(std::string filename, PAG::modelType modelType, int shaderProgramId); //Carga el modelo desde el nombre
        Model(std::string filename, int shaderProgramId);
        virtual ~Model();
        std::vector<GLfloat> *getVertices() const;
        std::vector<GLfloat> *getIndices() const;
        std::vector<GLfloat> *getNormals() const;
        std::vector<GLfloat> *getTextureCoordinates() const;
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
    };
} // PAG

#endif //PAG_1_MODEL_H

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
    enum modelType {BLANK,COLORED,TEXTURED};
    class Model {
    private:
        std::vector<GLfloat> *vertices;
        std::vector<GLfloat> *indices;
        std::vector<GLfloat> *colors;
        std::vector<GLfloat> *textureCoordinates;
        glm::mat4 modelMatrix;
        PAG::modelType modelType;
        GLuint shaderProgramId;
        GLuint idVAO = 0;
        GLuint idVBO_pos = 0;
        GLuint idVBO_colors = 0;
        GLuint idVBO_textureCoordinates = 0;
    public:
        Model(); //Crea el modelo del triángulo por defecto
        Model(std::string filename, PAG::modelType modelType, int shaderProgramId); //Carga el modelo desde el nombre
        virtual ~Model();

        std::vector<GLfloat> *getVertices() const;

        std::vector<GLfloat> *getIndices() const;

        std::vector<GLfloat> *getColors() const;

        std::vector<GLfloat> *getTextureCoordinates() const;

        const glm::mat4 &getModelMatrix() const;

        PAG::modelType getModelType() const;

        GLuint getShaderProgramId() const;

        GLuint getIdVao() const;

        GLuint getIdVboPos() const;

        GLuint getIdVboColors() const;

        GLuint getIdVboTextureCoordinates() const;

    };
} // PAG

#endif //PAG_1_MODEL_H

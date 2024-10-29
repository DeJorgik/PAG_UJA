//
// Created by DeJorgiK on 25/10/2024.
//

#include "Model.h"

namespace PAG {
    Model::~Model() {

    }

    /**
     * Crea el triángulo por defecto
     */
    Model::Model() {
        vertices = new std::vector<GLfloat>();
        vertices->push_back(-5);
        vertices->push_back(-5);
        vertices->push_back(0);
        vertices->push_back(5);
        vertices->push_back(-5);
        vertices->push_back(0);
        vertices->push_back(0);
        vertices->push_back(5);
        vertices->push_back(0);

        indices = new std::vector<GLfloat>();
        indices->push_back(0);
        indices->push_back(1);
        indices->push_back(2);

        colors = new std::vector<GLfloat>();
        colors->push_back(1);
        colors->push_back(0);
        colors->push_back(0);
        colors->push_back(0);
        colors->push_back(1);
        colors->push_back(0);
        colors->push_back(0);
        colors->push_back(0);
        colors->push_back(1);

        modelType = PAG::modelType::COLORED;
        shaderProgramId = 1;
    }

    Model::Model(std::string filename, PAG::modelType modelType, int shaderProgramId) {

    }

    const glm::mat4 &Model::getModelMatrix() const {
        return modelMatrix;
    }

    modelType Model::getModelType() const {
        return modelType;
    }

    GLuint Model::getShaderProgramId() const {
        return shaderProgramId;
    }

    GLuint Model::getIdVao() const {
        return idVAO;
    }

    GLuint Model::getIdVboPos() const {
        return idVBO_pos;
    }

    GLuint Model::getIdVboColors() const {
        return idVBO_colors;
    }

    GLuint Model::getIdVboTextureCoordinates() const {
        return idVBO_textureCoordinates;
    }

    std::vector<GLfloat> *Model::getVertices() const {
        return vertices;
    }

    std::vector<GLfloat> *Model::getIndices() const {
        return indices;
    }

    std::vector<GLfloat> *Model::getColors() const {
        return colors;
    }

    std::vector<GLfloat> *Model::getTextureCoordinates() const {
        return textureCoordinates;
    }


} // PAG
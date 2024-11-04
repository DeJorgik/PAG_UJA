//
// Created by DeJorgiK on 25/10/2024.
//

#include "Model.h"

namespace PAG {
    Model::~Model() {

    }

    void Model::drawDefaultTriangle(){
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

        normals = new std::vector<GLfloat>();
        normals->push_back(1);
        normals->push_back(0);
        normals->push_back(0);
        normals->push_back(0);
        normals->push_back(1);
        normals->push_back(0);
        normals->push_back(0);
        normals->push_back(0);
        normals->push_back(1);

        shaderProgramId = 1;
    }

    //PRACTICA 6 Sólo Posiciones y Normales
    Model::Model(std::string filename,int shaderProgramId){
        if (filename.empty()){
            drawDefaultTriangle();
        }else{

        }

    }

    const glm::mat4 &Model::getModelMatrix() const {
        return modelMatrix;
    }


    GLuint Model::getShaderProgramId() const {
        return shaderProgramId;
    }

    std::vector<GLfloat> *Model::getVertices() const {
        return vertices;
    }

    std::vector<GLfloat> *Model::getIndices() const {
        return indices;
    }

    std::vector<GLfloat> *Model::getNormals() const {
        return normals;
    }

    GLuint* Model::getIdVaoPointer() {
        return &idVAO;
    }

    GLuint Model::getIdVao() {
        return idVAO;
    }

    GLuint Model::getIdVboPos() {
        return idVBO_pos;
    }

    GLuint *Model::getIdVboPosPointer() {
        return &idVBO_pos;
    }

    GLuint Model::getIdVboNormals() {
        return idVBO_normals;
    }

    GLuint *Model::getIdVboNormalsPointer() {
        return &idVBO_normals;
    }

    GLuint Model::getIdIbo() {
        return idIBO;
    }

    GLuint *Model::getIdIboPointer() {
        return &idIBO;
    }


} // PAG
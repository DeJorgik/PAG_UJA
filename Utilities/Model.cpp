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
    //Se le pasa el nombre del archivo, el shader program se pone a parte
    Model::Model(std::string filename){
        if (filename.empty()){
            drawDefaultTriangle();
        }else{
            Assimp::Importer importer;
            const aiScene* assimpScene = importer.ReadFile ( filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

            if (!scene) {
                //Sistema de seguridad
                //Mostrar mensaje por pantalla
                return;
            }

            //Procesar la escena para guardar vertices y normales
            sceneProcess(assimpScene);
            
        }

    }

    //Función que procesa una escena importada por assimp
    void Model::sceneProcess(aiScene* assimpScene){
        //Procesar meshes de la escena
        for(unsigned int i = 0; i < assimpScene->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[i];
            meshProcess(mesh);
        }
    }

    //Función que procesa una mesh
    void Model::meshProcess(aiMesh* mesh){
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertices.push_back(vertex);
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

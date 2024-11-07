//
// Created by DeJorgiK on 25/10/2024.
//

#include "Model.h"

namespace PAG {
    Model::~Model() {
        
    }

    void Model::drawDefaultTriangle(){
        vertices = new std::vector<GLfloat>();
        vertices->push_back(-.5);
        vertices->push_back(-.5);
        vertices->push_back(0);
        vertices->push_back(.5);
        vertices->push_back(-.5);
        vertices->push_back(0);
        vertices->push_back(0);
        vertices->push_back(.5);
        vertices->push_back(0);

        indices = new std::vector<GLuint>();
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
    }

    //PRACTICA 6 Sólo Posiciones y Normales
    //Se le pasa el nombre del archivo, el shader program se pone a parte

    Model::Model(std::string filename){
        if (filename.empty()){
            drawDefaultTriangle();
        }else{
            Assimp::Importer importer;
            const aiScene* assimpScene = importer.ReadFile ( filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

            if (!assimpScene) {
                //Sistema de seguridad
                //Mostrar mensaje por pantalla
                return;
            }

            //Crear vectores
            vertices = new std::vector<GLfloat>();
            indices = new std::vector<GLuint>();
            normals = new std::vector<GLfloat>();

            //Procesar la escena para guardar vertices y normales
            sceneProcess(assimpScene);

        }

    }

    //Función que procesa una escena importada por assimp
    void Model::sceneProcess(const aiScene* assimpScene){
        //Procesar meshes de la escena
        for(unsigned int i = 0; i < assimpScene->mNumMeshes; i++) {
            aiMesh *mesh = assimpScene->mMeshes[i];
            meshProcess(mesh);
        }
    }

    //Función que procesa una mesh
    void Model::meshProcess(aiMesh* mesh){
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            //Procesar vertices
            aiVector3D vertex = mesh->mVertices[i];
            vertices->push_back(vertex.x);
            vertices->push_back(vertex.y);
            vertices->push_back(vertex.z);
            //Procesar normales
            aiVector3D normal = mesh->mNormals[i];
            normals->push_back(normal.x);
            normals->push_back(normal.y);
            normals->push_back(normal.z);
        }

        //Procesar indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices;j++){
                GLuint index = face.mIndices[j];
                indices->push_back(index);
            }
        }
    }

    
    const glm::mat4 &Model::getModelMatrix() const {
        return modelMatrix;
    }

    std::vector<GLfloat> *Model::getVertices() const {
        return vertices;
    }

    std::vector<GLuint> *Model::getIndices() const {
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

    GLfloat *Model::getVerticesArray() {
        auto * arr = new GLfloat [vertices->size()];
        std::copy(vertices->begin(), vertices->end(), arr);
        return arr;
    }

    GLfloat *Model::getNormalsArray() {
        auto * arr = new GLfloat [normals->size()];
        std::copy(normals->begin(), normals->end(), arr);
        return arr;
    }

    GLuint *Model::getIndicesArray() {
        auto * arr = new GLuint [indices->size()];
        std::copy(indices->begin(), indices->end(), arr);
        return arr;
    }


} // PAG

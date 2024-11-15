//
// Created by DeJorgiK on 25/10/2024.
//

#define GLM_ENABLE_EXPERIMENTAL
#include <filesystem>
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace PAG {

    Model::~Model() {
        vertices = nullptr;
        indices = nullptr;
        normals = nullptr;
    }

    void Model::drawDefaultTriangle(){
        name = "default_triangle";

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

    /**
     * crea un material por defecto, pinta de color rojo
     * TODO: cambiar esto para que se puedan customizar los materiales antes de crear el objeto
     */
    void Model::createDefaultMaterial(){
        material = new PAG::Material(name+"_material",glm::vec3(),glm::vec3(),glm::vec3(),1);
    }

    Model::Model(std::string filename){
        modelMatrix = glm::mat4(1.0f); //matriz de modelo por defecto
        if (filename.empty()){
            drawDefaultTriangle();
        }else {
            Assimp::Importer importer;
            const aiScene* assimpScene = importer.ReadFile ( filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

            if (!assimpScene) {
                //Sistema de seguridad
                //Mostrar mensaje por pantalla
                return;
            }

            //Dar nombre al modelo
            std::filesystem::path p(filename);
            name = p.stem().string();

            //Crear vectores
            vertices = new std::vector<GLfloat>();
            indices = new std::vector<GLuint>();
            normals = new std::vector<GLfloat>();

            //Procesar la escena para guardar vertices y normales
            sceneProcess(assimpScene);

            //Crear material por defecto
            createDefaultMaterial();
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

    /**
     * Aplica una transformación de traslación a la matriz de modelado
     * @param transform
     */
    void Model::modelTranslate(glm::vec3 transform){
        modelMatrix = glm::translate(modelMatrix,transform);
    }

    /**
     * Aplica una transformación de rotación en el eje definido
     * IMPORTANTE: EL ANGULO VIENE DADO EN GRADOS Y SE TRANSFORMA A RADIANES
     * @param axis
     * @param angle
     */
    void Model::modelRotate(glm::vec3 axis,float angle){
        modelMatrix = glm::rotate(modelMatrix,glm::radians(angle),axis);
    }

    /**
     * Aplica una trasnformación de escala
     * @param scale
     */
    void Model::modelScale(glm::vec3 scale){
        modelMatrix = glm::scale(modelMatrix,scale);
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

    std::string *Model::getModelName() {
        return &name;
    }

    void Model::setName(const std::string &name) {
        Model::name = name;
    }

    PAG::modelVisualizationTypes Model::getModelVisualizationType() const {
        return modelVisualizationType;
    }

    void Model::setModelVisualizationType(modelVisualizationTypes modelVisualizationType) {
        Model::modelVisualizationType = modelVisualizationType;
    }


} // PAG

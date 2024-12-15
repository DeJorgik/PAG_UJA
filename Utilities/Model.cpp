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
        textureCoodrinates = nullptr;
        if(idTexture!=0){glDeleteTextures(1,idTexture);} //Destruir textura del modelo
        idTexture = nullptr;
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

        textureCoodrinates = new std::vector<GLfloat>();
    }

    /**
     * crea un material por defecto, pinta de color rojo
     * TODO: cambiar esto para que se puedan customizar los materiales antes de crear el objeto
     */
    void Model::createMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                               const glm::vec3 &specular, GLfloat exponent){
        material = new PAG::Material(name+"_material",ambient,diffuse,specular,exponent);
    }
    Model::Model(std::string filename, modelVisualizationTypes modelVisualizationType):
            modelVisualizationType(modelVisualizationType){
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
            textureCoodrinates = new std::vector<GLfloat>();

            //Procesar la escena para guardar vertices y normales
            sceneProcess(assimpScene);

            //Crear material por defecto
            //createDefaultMaterial();

            //Crear id de textura
            idTexture = new GLuint();
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
            //Procesar coordenadas de textura
            if(mesh->mTextureCoords[0]){ //si hay texturas
                //auto textureCoordinte = mesh->mTextureCoords[0][i];
                textureCoodrinates->push_back(mesh->mTextureCoords[0][i].x);
                textureCoodrinates->push_back(mesh->mTextureCoords[0][i].y);
            }else{
                textureCoodrinates->push_back(0);
                textureCoodrinates->push_back(0);
            }
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

    void Model::loadTexture(std::string filename){

        //Decodificar
        unsigned error = lodepng::decode (texturePixels,
                                          textureWidth,textureHeight,
                                          "../Textures/"+filename+".png");
        if (error){
            throw std::runtime_error("ERROR: ../Textures/"+filename+".png cannot be loaded.");
        }

        //variables auxiliares
        unsigned char *texturePtr = &texturePixels[0];
        int textureColorComponents = 4;
        int widthIncrement = textureWidth * textureColorComponents; // Ancho en bytes
        unsigned char* top = nullptr;
        unsigned char* bot = nullptr;
        unsigned char temp = 0;

        //Cargar textura y dar la vuelta
        for (int i = 0; i < textureHeight / 2; i++){
            top = texturePtr + i * widthIncrement;
            bot = texturePtr + (textureHeight - i - 1) * widthIncrement;
            for (int j = 0; j < widthIncrement; j++){
                temp = *top;
                *top = *bot;
                *bot = temp;
                ++top;
                ++bot;
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

    GLuint Model::getIdVboTextureCoodrinates() {
        return idVBO_textureCoordinates;
    }

    GLuint *Model::getIdVboTextureCoodrinatesPointer() {
        return &idVBO_textureCoordinates;
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

    GLuint *Model::getTextureCoordinatesArray() {
        auto * arr = new GLuint [textureCoodrinates->size()];
        std::copy(textureCoodrinates->begin(), textureCoodrinates->end(), arr);
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

    Material *Model::getMaterial() const {
        return material;
    }

    /**
     * Función que se usa para cambiar el modo de visualización
     * @param isWireframe
     */
    void Model::setWireframe(bool isWireframe) {
        if(isWireframe) modelVisualizationType = modelVisualizationTypes::WIREFRAME;
        else  modelVisualizationType = modelVisualizationTypes::FILL;
    }

    const std::vector<unsigned char> &Model::getTexturePixels() const {
        return texturePixels;
    }

    void Model::setTexturePixels(const std::vector<unsigned char> &texturePixels) {
        Model::texturePixels = texturePixels;
    }

    unsigned int Model::getTextureWidth() const {
        return textureWidth;
    }

    void Model::setTextureWidth(unsigned int textureWidth) {
        Model::textureWidth = textureWidth;
    }

    unsigned int Model::getTextureHeight() const {
        return textureHeight;
    }

    void Model::setTextureHeight(unsigned int textureHeight) {
        Model::textureHeight = textureHeight;
    }

    std::vector<GLfloat> *Model::getTextureCoodrinates() const {
        return textureCoodrinates;
    }

    GLuint * Model::getIdTexture() const {
        return idTexture;
    }


} // PAG

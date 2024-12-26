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
        textureCoordinates = nullptr;
        idTexture = nullptr;
        idNormalMap = nullptr;
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

        textureCoordinates = new std::vector<GLfloat>();
    }

    /**
     * crea un material por defecto, pinta de color rojo
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
            const aiScene* assimpScene = importer.ReadFile ( filename, aiProcess_JoinIdenticalVertices
                                                                            | aiProcess_Triangulate
                                                                            | aiProcess_GenSmoothNormals
                                                                            | aiProcess_CalcTangentSpace);

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
            textureCoordinates = new std::vector<GLfloat>();
            tangents = new std::vector<GLfloat>();
            bitangents = new std::vector<GLfloat>();

            //Procesar la escena para guardar vertices y normales
            sceneProcess(assimpScene);

            //Crear id de textura
            idTexture = new GLuint();
            idNormalMap = new GLuint();
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
                textureCoordinates->push_back(mesh->mTextureCoords[0][i].x);
                textureCoordinates->push_back(mesh->mTextureCoords[0][i].y);
            }else{
                textureCoordinates->push_back(0);
                textureCoordinates->push_back(0);
            }
            //Pocesar tangentes y bitangentes
            if (mesh->mTangents){
                aiVector3D tangent = mesh->mTangents[i];
                tangents->push_back(tangent.x);
                tangents->push_back(tangent.y);
                tangents->push_back(tangent.z);
                aiVector3D bitangent = mesh->mBitangents[i];
                bitangents->push_back(bitangent.x);
                bitangents->push_back(bitangent.y);
                bitangents->push_back(bitangent.z);
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

    /**
     * Carga las texturas utilizado la librería assimp
     * @param filename
     */
    void Model::loadTexture(std::string filename){

        if(filename!=textureName){
            textureName = filename;
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

            //Crear textura en OpenGL
            createTextureOpenGL();
        }
    }

    /**
     * Lo mismo pero con normal map
     * @param filename
     */
    void Model::loadNormalMap(std::string filename){

        if(filename!=normalMapName){
            normalMapName = filename;
            //Decodificar
            unsigned error = lodepng::decode (normalMapPixels,
                                              normalMapWidth,normalMapHeight,
                                              "../Textures/"+filename+".png");
            if (error){
                throw std::runtime_error("ERROR: ../Textures/"+filename+".png cannot be loaded.");
            }

            //variables auxiliares
            unsigned char *texturePtr = &normalMapPixels[0];
            int textureColorComponents = 4;
            int widthIncrement = normalMapWidth * textureColorComponents; // Ancho en bytes
            unsigned char* top = nullptr;
            unsigned char* bot = nullptr;
            unsigned char temp = 0;

            //Cargar textura y dar la vuelta
            for (int i = 0; i < normalMapHeight / 2; i++){
                top = texturePtr + i * widthIncrement;
                bot = texturePtr + (normalMapHeight - i - 1) * widthIncrement;
                for (int j = 0; j < widthIncrement; j++){
                    temp = *top;
                    *top = *bot;
                    *bot = temp;
                    ++top;
                    ++bot;
                }
            }

            //Crear textura en OpenGL
            createNormalMapOpenGL();
        }
    }

    //FUNCIONES OPENGL

    /**
     * Función para crear y enlazar el VAO del modelo
     * Esta función estaba antes en el renderer, pero es mas conveniente ubicarla en el mismo modelo
     */
    void Model::createVaoOpenGL(){

        //Generar VAO
        glGenVertexArrays (1, &idVAO);
        glBindVertexArray(idVAO);

        //VBO posición
        glGenBuffers(1, &idVBO_pos);
        glBindBuffer(GL_ARRAY_BUFFER,idVBO_pos);
        glBufferData(GL_ARRAY_BUFFER,vertices->size()*sizeof(GLfloat),vertices->data(),GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr);
        glEnableVertexAttribArray(0);

        //VBO Normales
        glGenBuffers(1, &idVBO_normals);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO_normals);
        glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(GLfloat), normals->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr);
        glEnableVertexAttribArray(1);

        //VBO Coordenadas de Textura
        glGenBuffers(1, &idVBO_textureCoordinates);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO_textureCoordinates);
        glBufferData(GL_ARRAY_BUFFER, textureCoordinates->size() * sizeof(GLfloat), textureCoordinates->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),nullptr);
        glEnableVertexAttribArray(2);

        //VBO Tangentes
        glGenBuffers(1, &idVBO_tangents);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO_tangents);
        glBufferData(GL_ARRAY_BUFFER, tangents->size() * sizeof(GLfloat), tangents->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr);
        glEnableVertexAttribArray(3);

        //VBO Bitangentes
        glGenBuffers(1, &idVBO_bitangents);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO_bitangents);
        glBufferData(GL_ARRAY_BUFFER, bitangents->size() * sizeof(GLfloat), bitangents->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr);
        glEnableVertexAttribArray(4);

        //Generar IBO
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices->size()*sizeof(GLfloat),indices->data(),GL_STATIC_DRAW);
    }

    /**
     * Crea la textura cargada en opengl
     */
    void Model::createTextureOpenGL(){
        //Cargar textura en openGL
        glGenTextures(1,idTexture);
        glBindTexture(GL_TEXTURE_2D,*idTexture);

        // Cómo resolver la minificación.
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        // Cómo resolver la magnificación.
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        // Cómo pasar de coordenadas de textura a coordenadas en el espacio de la textura en horizontal
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        // Cómo pasar de coordenadas de textura a coordenadas en el espacio de la textura en vertical
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        // Transferimos la información de la imagen. En este caso, la imagen está guardada en std::vector<unsigned char> _pixels;
        glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA,
                       textureWidth,
                       textureHeight,
                       0, GL_RGBA, GL_UNSIGNED_BYTE,
                       texturePixels.data());

        //Generar mipmap finalmente
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    /**
     * Similar pero para mapas de normales
     */
    void Model::createNormalMapOpenGL(){
        //Cargar textura en openGL
        glGenTextures(1,idNormalMap);
        glBindTexture(GL_TEXTURE_2D,*idNormalMap);

        // Cómo resolver la minificación.
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        // Cómo resolver la magnificación.
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        // Cómo pasar de coordenadas de textura a coordenadas en el espacio de la textura en horizontal
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        // Cómo pasar de coordenadas de textura a coordenadas en el espacio de la textura en vertical
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        // Transferimos la información de la imagen. En este caso, la imagen está guardada en std::vector<unsigned char> _pixels;
        glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA,
                       normalMapWidth,
                       normalMapHeight,
                       0, GL_RGBA, GL_UNSIGNED_BYTE,
                       normalMapPixels.data());

        //Generar mipmap finalmente
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    //TRANSFORMACIONES MODELADO

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

    //GETTERS

    const glm::mat4 &Model::getModelMatrix() const {
        return modelMatrix;
    }

    std::vector<GLuint> *Model::getIndices() const {
        return indices;
    }

    GLuint Model::getIdVao() {
        return idVAO;
    }

    GLuint Model::getIdIbo() {
        return idIBO;
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

    GLuint * Model::getIdTexture() const {
        return idTexture;
    }

    GLuint *Model::getIdNormalMap() const {
        return idNormalMap;
    }

} // PAG

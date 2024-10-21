//
// Created by DeJorgiK on 11/10/2024.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <glm/gtc/epsilon.hpp>
#include "Camera.h"

namespace PAG {

    Camera::Camera(const glm::vec3 &cameraPos, const glm::vec3 &lookAtPoint, const glm::vec3 &up, float fovY,
                   float zNear, float zFar, float aspectRatio, const glm::vec3 Y) : cameraPos(cameraPos), lookAtPoint(lookAtPoint), up(up),
                                                                 fovY(fovY), zNear(zNear), zFar(zFar),
                                                                 aspectRatio(aspectRatio), Y(Y) {
        //Obtener sistema de coordenadas
        updateCoordinateSystem();
        //Transformaciones
        translation = glm::vec3(0,0,0);
        rotationX = 0.f;
        rotationY = 0.f;
        rotationZ = 0.f;
    }

    const glm::vec3 &Camera::getCameraPos() const {
        return cameraPos;
    }

    void Camera::setCameraPos(const glm::vec3 &cameraPos) {
        Camera::cameraPos = cameraPos;
    }

    const glm::vec3 &Camera::getLookAtPoint() const {
        return lookAtPoint;
    }

    void Camera::setLookAtPoint(const glm::vec3 &lookAtPoint) {
        Camera::lookAtPoint = lookAtPoint;
    }

    float Camera::getFovY() const {
        return fovY;
    }

    void Camera::setFovY(float fovY) {
        Camera::fovY = fovY;
    }

    float Camera::getZNear() const {
        return zNear;
    }

    void Camera::setZNear(float zNear) {
        Camera::zNear = zNear;
    }

    float Camera::getZFar() const {
        return zFar;
    }

    void Camera::setZFar(float zFar) {
        Camera::zFar = zFar;
    }

    float Camera::getAspectRatio() const {
        return aspectRatio;
    }

    void Camera::setAspectRatio(float aspectRatio) {
        Camera::aspectRatio = aspectRatio;
    }

    /**
     * Calcula la matriz de visión
     * @return
     */
    glm::mat4 Camera::calculateViewMatrix() {
        //Traslación
        //glm::mat4 viewTranslated = glm::translate(view,translation);

        //Rotación
        //GLfloat rotationXRadians = glm::radians(rotationX);

        // Tran
        glm::mat4 view = glm::lookAt(cameraPos,lookAtPoint,up);
        updateCoordinateSystem();
        return view;
    }

    /**
     * Calcula la matriz de proyección en perspectiva
     * @return
     */
    glm::mat4 Camera::calculateProjectionMatrix() {
        GLfloat fovYRadians = glm::radians(fovY); //Pasar a radianes
        return glm::perspective(fovYRadians,aspectRatio,zNear,zFar);
    }

    /**
     * Actualiza el aspect ratio
     * @param width
     * @param height
     */
    void Camera::updateAspectRatio(int width, int height) {
        aspectRatio = width/height;
    }

    /**
     * Actualiza el zoom
     * @param zoom
     */
    void Camera::updateZoom(float zoom){
        fovY = zoom;
    }

    /**
     * Añade traslación a la cámata
     * @param translate
     */
    void Camera::addTranslation(glm::vec3 translate) {
        translation = translation+translate;
    }

    /**
     * Añade Rotación a la cámara (depdende del eje)
     * @param translate
     */
    void Camera::updateRotationY(float angle) {
        rotationY=angle;
    }

    void Camera::updateRotationX(float angle) {
        rotationX = angle;
    }

    /**
     * Movimiento de PAN: Rotación del punto lookat alrededor del eje v
     * @param angle
     */
    void Camera::panMovement(float angle){
        rotationY = angle;
        //PAN EJE Y
        GLfloat rotationYRadians = glm::radians(rotationY); //Pasar a radianes
        //-T * R * T
        //Mover al origen
        glm::mat4 t1 = glm::translate(-cameraPos);
        //Rotar alrededor de eje v
        glm::mat4 r = glm::rotate(rotationYRadians,v);
        //Devolver a su sitio
        glm::mat4 t2 = glm::translate(cameraPos);
        lookAtPoint = t2*r*t1*glm::vec4(lookAtPoint,1.0f);
    }

    /**
     * Movimiento de TILT: Rotación del punto lookat alrededor del eje u
     */
    void Camera::tiltMovement(float angle){
        rotationX = angle;
        GLfloat rotationYRadians = glm::radians(rotationX); //Pasar a radianes
        //-T * R * T
        //Mover al origen
        glm::mat4 t1 = glm::translate(-cameraPos);
        //Rotar alrededor de eje v
        glm::mat4 r = glm::rotate(rotationYRadians,u);
        //Devolver a su sitio
        glm::mat4 t2 = glm::translate(cameraPos);
        lookAtPoint = t2*r*t1*glm::vec4(lookAtPoint,1.0f);
    }

    /**
     * Movimiento de DOLLY/CRANE: traslación de lookat y de posición en los ejes x y z
     */
     void Camera::dollyCraneMovement(glm::vec3 translate){
         translation = translate;
         glm::mat4 t1 = glm::translate(translate);
         lookAtPoint = t1*glm::vec4(lookAtPoint,1.0f);
         cameraPos = t1*glm::vec4(cameraPos,1.0f);
    }

    /**
     * Actualiza el sistema de coordenadas local UVN
     */
    void Camera::updateCoordinateSystem() {
        v = up;
        n = glm::normalize(-(cameraPos-lookAtPoint));
        //Si n e Y son iguales
        if(all(glm::epsilonEqual(n,Y,glm::epsilon<float>()))){
            u = glm::cross(glm::vec3(0,0,1),n);
        } else if (all(glm::epsilonEqual(n,-Y,glm::epsilon<float>()))){
            u = glm::cross(glm::vec3(0,0,-1),n);
        }else{
            u = glm::cross(Y,n);
        }
    }

} // PAG
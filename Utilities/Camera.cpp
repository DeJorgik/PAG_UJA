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
    }

    /**
     * Calcula la matriz de visión
     * @return
     */
    glm::mat4 Camera::calculateViewMatrix() {
        glm::mat4 view = glm::lookAt(cameraPos,lookAtPoint,up);
        updateCoordinateSystem();//actualizar sistema de coordenadas constatemete
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
        aspectRatio = float(width)/float(height);
    }

    /**
     * Actualiza el zoom
     * @param zoom
     */
    void Camera::updateZoom(float zoom){
        fovY = zoom;
    }

    void Camera::addZoom(float zoom){
        fovY += zoom;
        if(fovY<1){
            fovY=1;
        }
        if(fovY>120){
            fovY=120;
        }
    }

    /**
     * Movimiento de PAN: Rotación del punto lookat alrededor del eje v
     * @param angle
     */
    void Camera::panMovement(float angle){
        GLfloat rotationYRadians = glm::radians(angle); //Pasar a radianes
        //T * R * -T
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
        GLfloat rotationYRadians = glm::radians(angle);
        glm::mat4 t1 = glm::translate(-cameraPos);
        glm::mat4 r = glm::rotate(rotationYRadians,u);
        glm::mat4 t2 = glm::translate(cameraPos);
        lookAtPoint = t2*r*t1*glm::vec4(lookAtPoint,1.0f);
    }

    /**
     * Movimiento de DOLLY/CRANE: traslación de lookat y de posición en los ejes x y z
     */
     void Camera::dollyCraneMovement(glm::vec3 translate){
        glm::mat4 t;
         if(translate.x!=0){
             t = glm::translate(u*translate.x);
         } else if(translate.y!=0){
             t = glm::translate(v*translate.y);
         } else if(translate.z!=0){
             t = glm::translate(n*translate.z);
         }
         lookAtPoint = t*glm::vec4(lookAtPoint,1.0f);
         cameraPos = t*glm::vec4(cameraPos,1.0f);
    }

    /**
     * Movimiento de ORBIT: rotación de la posición alrededor del LookAt
     * longitud: respecto eje u
     * latitud: respecto eje v
     * @param longitude
     * @param latitude
     */
    void Camera::orbitMovement(float longitude,float latitude){
        GLfloat rotationLongitudeRadians = glm::radians(longitude);
        GLfloat rotationLatitudeRadians = glm::radians(latitude);
        glm::mat4 t1 = glm::translate(-lookAtPoint);
        glm::mat4 r1 = glm::rotate(rotationLongitudeRadians,u);
        glm::mat4 r2 = glm::rotate(rotationLatitudeRadians,v);
        glm::mat4 t2 = glm::translate(lookAtPoint);
        cameraPos = t2*r2*r1*t1*glm::vec4(cameraPos,1.0f);
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

    void Camera::setCameraPos(const glm::vec3 &cameraPos) {
        Camera::cameraPos = cameraPos;
    }

    void Camera::setLookAtPoint(const glm::vec3 &lookAtPoint) {
        Camera::lookAtPoint = lookAtPoint;
    }

    void Camera::setUp(const glm::vec3 &up) {
        Camera::up = up;
    }

    void Camera::setFovY(float fovY) {
        Camera::fovY = fovY;
    }

    void Camera::setZNear(float zNear) {
        Camera::zNear = zNear;
    }

    void Camera::setZFar(float zFar) {
        Camera::zFar = zFar;
    }

    void Camera::setAspectRatio(float aspectRatio) {
        Camera::aspectRatio = aspectRatio;
    }

    void Camera::setY(const glm::vec3 &y) {
        Y = y;
    }

} // PAG
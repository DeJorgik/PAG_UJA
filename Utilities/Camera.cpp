//
// Created by DeJorgiK on 11/10/2024.
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Camera.h"

namespace PAG {

    Camera::Camera(const glm::vec3 &cameraPos, const glm::vec3 &lookAtPoint, const glm::vec3 &up, float fovY,
                   float zNear, float zFar, float aspectRatio) : cameraPos(cameraPos), lookAtPoint(lookAtPoint), up(up),
                                                                 fovY(fovY), zNear(zNear), zFar(zFar),
                                                                 aspectRatio(aspectRatio) {}

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
    glm::mat4 Camera::calculateVisionMatrix() {
        return glm::lookAt(cameraPos,lookAtPoint,up);
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

} // PAG
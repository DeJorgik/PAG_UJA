#ifndef PAG_1_CAMERA_H
#define PAG_1_CAMERA_H

#include <glm/glm.hpp>

namespace PAG {

    class Camera {
    private:
        //Sistema de coordenadas local
        glm::vec3 u;
        glm::vec3 v;
        glm::vec3 n;
        glm::vec3 Y;
        //Matriz Visión
        glm::vec3 cameraPos;
        glm::vec3 lookAtPoint;
        glm::vec3 up;
        //Matriz Proyeccion
        float fovY; //yo lo pongo en ángulos y luego lo paso a radianes
        float zNear;
        float zFar;
        float aspectRatio;
    public:
        Camera(const glm::vec3 &cameraPos, const glm::vec3 &lookAtPoint, const glm::vec3 &up, float fovY, float zNear,
               float zFar, float aspectRatio, const glm::vec3 Y);
        glm::mat4 calculateViewMatrix();
        glm::mat4 calculateProjectionMatrix();
        void updateAspectRatio(int width, int height);
        void updateCoordinateSystem();
        const glm::vec3 &getCameraPos() const;
        void setCameraPos(const glm::vec3 &cameraPos);
        const glm::vec3 &getLookAtPoint() const;
        void setLookAtPoint(const glm::vec3 &lookAtPoint);
        float getFovY() const;
        void setFovY(float fovY);
        float getZNear() const;
        void setZNear(float zNear);
        float getZFar() const;
        void setZFar(float zFar);
        float getAspectRatio() const;
        void setAspectRatio(float aspectRatio);

        void panMovement(float angle);
        void tiltMovement(float angle);
        void dollyCraneMovement(glm::vec3 translate);
        void updateZoom(float zoom);
        void orbitMovementX(float longitude);
        void orbitMovementY(float latitude);

        void orbitMovement(float longitude, float latitude);
    };

} // PAG

#endif //PAG_1_CAMERA_H

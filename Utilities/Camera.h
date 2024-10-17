#ifndef PAG_1_CAMERA_H
#define PAG_1_CAMERA_H

#include <glm/glm.hpp>

namespace PAG {

    class Camera {
    private:
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
               float zFar, float aspectRatio);
        glm::mat4 calculateVisionMatrix();
        glm::mat4 calculateProjectionMatrix();
        void updateAspectRatio(int width, int height);
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

        void updateZoom(float zoom);
    };

} // PAG

#endif //PAG_1_CAMERA_H

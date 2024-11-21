//
// Created by DeJorgiK on 08/11/2024.
//

#ifndef PAG_1_MATERIAL_H
#define PAG_1_MATERIAL_H

#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <string>

namespace PAG {
    //Clase que representa un material
    class Material {
    private:
        std::string materialName; //nombre del material
        //Colores del material
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        GLfloat exponent; //Exponente para los brillos
    public:
        Material(const std::string &materialName, const glm::vec3 &ambient, const glm::vec3 &diffuse,
                 const glm::vec3 &specular, GLfloat exponent);

        const std::string &getMaterialName() const;

        const glm::vec3 &getAmbient() const;

        const glm::vec3 &getDiffuse() const;

        const glm::vec3 &getSpecular() const;

        GLfloat getExponent() const;

        GLfloat *getMaterialAmbient();

        GLfloat *getMaterialDiffuse();

        GLfloat *getMaterialSpecular();
    };

} // PAG

#endif //PAG_1_MATERIAL_H

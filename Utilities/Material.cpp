//
// Created by DeJorgiK on 08/11/2024.
//

#include "Material.h"

namespace PAG {
    Material::Material(const std::string &materialName, const glm::vec3 &ambient, const glm::vec3 &diffuse,
                       const glm::vec3 &specular, GLfloat exponent) : materialName(materialName), ambient(ambient),
                                                                      diffuse(diffuse), specular(specular),
                                                                      exponent(exponent) {}

    const std::string &Material::getMaterialName() const {
        return materialName;
    }

    const glm::vec3 &Material::getAmbient() const {
        return ambient;
    }

    const glm::vec3 &Material::getDiffuse() const {
        return diffuse;
    }

    const glm::vec3 &Material::getSpecular() const {
        return specular;
    }

    GLfloat Material::getExponent() const {
        return exponent;
    }

    void Material::setAmbient(const glm::vec3 &ambient) {
        Material::ambient = ambient;
    }

    void Material::setDiffuse(const glm::vec3 &diffuse) {
        Material::diffuse = diffuse;
    }

    void Material::setSpecular(const glm::vec3 &specular) {
        Material::specular = specular;
    }

    void Material::setExponent(GLfloat exponent) {
        Material::exponent = exponent;
    }
} // PAG
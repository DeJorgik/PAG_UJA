//
// Created by DeJorgiK on 26/11/2024.
//

#include "Light.h"

const std::string &Light::getLightName() const {
    return lightName;
}

void Light::setLightName(const std::string &lightName) {
    Light::lightName = lightName;
}

Light::lightTypes Light::getLightType() const {
    return lightType;
}

void Light::setLightType(Light::lightTypes lightType) {
    Light::lightType = lightType;
}

float Light::getIa() const {
    return Ia;
}

void Light::setIa(float ia) {
    Ia = ia;
}

float Light::getId() const {
    return Id;
}

void Light::setId(float id) {
    Id = id;
}

float Light::getIs() const {
    return Is;
}

void Light::setIs(float is) {
    Is = is;
}

const glm::vec3 &Light::getPos() const {
    return pos;
}

void Light::setPos(const glm::vec3 &pos) {
    Light::pos = pos;
}

const glm::vec3 &Light::getD() const {
    return d;
}

void Light::setD(const glm::vec3 &d) {
    Light::d = d;
}

float Light::getGamma() const {
    return gamma;
}

void Light::setGamma(float gamma) {
    Light::gamma = gamma;
}

float Light::getS() const {
    return s;
}

void Light::setS(float s) {
    Light::s = s;
}

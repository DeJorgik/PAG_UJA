//
// Created by DeJorgiK on 26/11/2024.
//

#include "Light.h"
namespace PAG {


    /**
     * El constructor lo inicializa a 0, luego se escogen los valores con otras funciones
     */
    Light::Light() {
        /*
        lightName="";
        lightType=lightTypes::AMBIENT;//Por defecto es ambiente
        Ia = glm::vec3(0,0,0);
        Id = glm::vec3(0,0,0);
        Is = glm::vec3(0,0,0);
        pos = glm::vec3(0,0,0);
        d = glm::vec3(0,0,0);
        gamma = 0;
        s = 0;*/
    }

    void Light::setAmbientLight(std::string _lightName, glm::vec3 _Ia){
        lightType=lightTypes::AMBIENT;
        lightName = _lightName;
        Ia = _Ia;
    }
    void Light::setAmbientLight(glm::vec3 _Ia){
        lightType=lightTypes::AMBIENT;
        Ia = _Ia;
    }

    void Light::setDirectionLight(std::string _lightName, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _d){
        lightType=lightTypes::DIRECTION;
        lightName = _lightName;
        Id = _Id;
        Is = _Is;
        d = _d;
    }

    void Light::setDirectionLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _d){
        lightType=lightTypes::DIRECTION;
        Id = _Id;
        Is = _Is;
        d = _d;
    }

    void Light::setPointLight(std::string _lightName, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _pos){
        lightType=lightTypes::POINT;
        lightName = _lightName;
        Id = _Id;
        Is = _Is;
        pos = _pos;
    }

    void Light::setPointLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _pos){
        lightType=lightTypes::POINT;
        Id = _Id;
        Is = _Is;
        pos = _pos;
    }

    void Light::setSpotLight(std::string _lightName, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _pos,glm::vec3 _d,float _gamma,float _s){
        lightType=lightTypes::SPOT;
        lightName = _lightName;
        Id = _Id;
        Is = _Is;
        pos = _pos;
        d = _d;
        gamma = _gamma;
        s = _s;
    }

    void Light::setSpotLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _pos,glm::vec3 _d,float _gamma,float _s){
        lightType=lightTypes::SPOT;
        Id = _Id;
        Is = _Is;
        pos = _pos;
        d = _d;
        gamma = _gamma;
        s = _s;
    }


    const std::string &Light::getLightName() const {
        return lightName;
    }

    void Light::setLightName(const std::string &lightName) {
        Light::lightName = lightName;
    }

    const glm::vec3 &Light::getPos() const {
        return pos;
    }

    const glm::vec3 &Light::getD() const {
        return d;
    }

    float Light::getGamma() const {
        return gamma;
    }

    const glm::vec3 &Light::getIa() const {
        return Ia;
    }

    const glm::vec3 &Light::getId() const {
        return Id;
    }

    const glm::vec3 &Light::getIs() const {
        return Is;
    }

    lightTypes Light::getLightType() const {
        return lightType;
    }

}
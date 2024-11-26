//
// Created by DeJorgiK on 26/11/2024.
//

#ifndef PAG_1_LIGHT_H
#define PAG_1_LIGHT_H

#include <string>
#include <glm/vec3.hpp>

/**
 * Clase que representa las luces
 */
class Light {
    enum lightTypes{AMBIENT,DIRECTION,POINT,SPOT};
private:
    std::string lightName;
    lightTypes lightType;
    float Ia, Id, Is; //Color ambiente, difusa y especular
    glm::vec3 pos; //Posición
    glm::vec3 d; //Dirección (OJO DESDE LA FUENTE HACI LA ESCENA)
    float gamma; //Ángulo de cono de luz
    float s; //Exponente de bordes suaves en el cono de luz

public:

    const std::string &getLightName() const;
    void setLightName(const std::string &lightName);
    lightTypes getLightType() const;
    void setLightType(lightTypes lightType);
    float getIa() const;
    void setIa(float ia);
    float getId() const;
    void setId(float id);
    float getIs() const;
    void setIs(float is);
    const glm::vec3 &getPos() const;
    void setPos(const glm::vec3 &pos);
    const glm::vec3 &getD() const;
    void setD(const glm::vec3 &d);
    float getGamma() const;
    void setGamma(float gamma);
    float getS() const;
    void setS(float s);
};


#endif //PAG_1_LIGHT_H

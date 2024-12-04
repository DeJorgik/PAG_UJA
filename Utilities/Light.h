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
namespace PAG{
    enum lightTypes{AMBIENT,DIRECTION,POINT,SPOT};
    class Light {
    private:
        std::string lightName;
        lightTypes lightType;
        glm::vec3 Ia, Id, Is; //Color ambiente, difusa y especular
        glm::vec3 pos; //Posición
        glm::vec3 d; //Dirección (OJO DESDE LA FUENTE HACIA LA ESCENA)
        float gamma; //Ángulo de cono de luz
        float s; //Exponente de bordes suaves en el cono de luz
    public:
        Light();

        const std::string &getLightName() const;
        void setLightName(const std::string &lightName);
        lightTypes getLightType() const;
        void setLightType(lightTypes lightType);
        const glm::vec3 &getIa() const;
        void setIa(const glm::vec3 &ia);
        const glm::vec3 &getId() const;
        void setId(const glm::vec3 &id);
        const glm::vec3 &getIs() const;
        void setIs(const glm::vec3 &is);
        const glm::vec3 &getPos() const;
        void setPos(const glm::vec3 &pos);
        const glm::vec3 &getD() const;
        void setD(const glm::vec3 &d);
        float getGamma() const;
        void setGamma(float gamma);
        float getS() const;
        void setS(float s);

        void setAmbientLight(std::string _lightName, glm::vec3 _Ia);
        void setAmbientLight(glm::vec3 _Ia);

        void setDirectionLight(std::string _lightName, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _d);
        void setDirectionLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _d);

        void setPointLight(std::string _lightName, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _pos);
        void setPointLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _pos);

        void
        setSpotLight(std::string _lightName, glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _pos, glm::vec3 _d, float _gamma,
                     float _s);
        void
        setSpotLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _pos, glm::vec3 _d, float _gamma,
                     float _s);
    };

}

#endif //PAG_1_LIGHT_H

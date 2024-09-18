#ifndef PAG_1_RENDERER_H
#define PAG_1_RENDERER_H

#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Clase que encapsula el dibujado de la pantalla

namespace PAG {

    class Renderer {
    public:
        Renderer();

    private:
        void referescarVentana(GLFWwindow *window);
    };

} // PAG

#endif //PAG_1_RENDERER_H

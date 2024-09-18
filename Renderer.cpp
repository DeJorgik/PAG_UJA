#include "Renderer.h"

namespace PAG {
    void Renderer::referescarVentana(GLFWwindow *window) {
        // - Borra los buffers (color y profundidad)
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
        // intercambia el buffer back (en el que se ha estado dibujando) por el
        // que se mostraba hasta ahora (front).
        glfwSwapBuffers(window); //OJO TIENE QUE SER LA ULTIMA ORDEN EN EL CALLBACK
        std::cout << "REFRESH CALLBACK CALLED" << std::endl;
    }

    Renderer::Renderer() {}
} // PAG
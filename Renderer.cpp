#include "Renderer.h"

namespace PAG {
    void Renderer::referescarVentana(GLFWwindow *window) {
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glfwSwapBuffers(window);
    }

    Renderer::Renderer() {}
} // PAG
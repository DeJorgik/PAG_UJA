#include "Renderer.h"

namespace PAG {

    Renderer::Renderer() {
        //inicializa el fondo a gris
        bg_color[0] = 0.5f;
        bg_color[1] = 0.5f;
        bg_color[2] = 0.5f;
    }

    Renderer::~Renderer() {

    }

    Renderer &Renderer::getInstance() {
        if (!rederer_instance){
            rederer_instance = new Renderer;
        }

        return *rederer_instance;
    }

    void Renderer::rendererInit() {
        //Mostrar contexto ya definido
        std::cout << glGetString ( GL_RENDERER ) << std::endl
                  << glGetString ( GL_VENDOR ) << std::endl
                  << glGetString ( GL_VERSION ) << std::endl
                  << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

        glEnable(GL_DEPTH_TEST);

        updateBgColor();

    }

    void Renderer::windowRefresh() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void Renderer::viewportResize(int width, int height) {
        glViewport(0,0,width,height);
    }

    void Renderer::updateBgColor() {
        glClearColor(bg_color[0],bg_color[1],bg_color[2],1.0f);
    }

    float Renderer::getBgColor(int color_id) {
        return bg_color[color_id];
    }

    void Renderer::setBgColor(int color_id, float value) {
        bg_color[color_id] = value;
    }
} // PAG
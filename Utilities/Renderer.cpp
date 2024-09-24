#include "Renderer.h"

namespace PAG {

    PAG::Renderer* PAG::Renderer::rederer_instance = nullptr;

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

    GLfloat *Renderer::getBgColor() {
        return bg_color;
    }

    void Renderer::setBgColor(int color_id, float value) {
        bg_color[color_id] = value;
        updateBgColor();
    }

    void Renderer::addBgColor(int color_id, double increment) {

        //Acotando (mas facil)
        bg_color[color_id]+=increment;

        if(bg_color[color_id]<0){
            bg_color[color_id] = 0.0f;
        } else if (bg_color[color_id]>1){
            bg_color[color_id] = 1.0f;
        }

        /*

        Truncado (mas complicado luego lo termino)

        GLfloat final_value;
        GLfloat difference;

        final_value = bg_color[color_id]+increment;
        if(final_value<0||final_value>1){
            difference = (final_value-1)/1; //obtener diferencia porcentual
            if difference>1

            if(final_value<0) bg_color[color_id] = 0.0f;
            else bg_color[color_id] = 1.0f;

            for (int i = 0; i < 3; ++i) {
                if (i!=color_id){
                    bg_color[i]*=difference;
                }
            }

        } else {
            bg_color[color_id] = final_value;
        }*/

        updateBgColor();
    }
} // PAG
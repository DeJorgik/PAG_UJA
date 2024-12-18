// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW


#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Utilities/Renderer.h"
#include "Utilities/GUI.h"

//PRÁCTICA 1: variable global del modo de cambiar de fondo
int mode = -1; //Modo, rojo, verde, azul
//PRÁCTICA 2 buffer con el texto que aparecerá por patalla

//PRÁCTICA 5 variables para la dimensión del viewport
int width = 1024;
int height = 576;

bool mouseCameraCtr = false; //variable para controlar la camara con el ratón
double mousePosX, mousePosY; //posicion del ratón

/**
 * Función que muestra por pantalla los errores que ocurren
 * @param _errno
 * @param desc
 */
void error_callback(int errno, const char* desc){
    std::string aux(desc);
    PAG::GUI::getInstance().messageBufferAdd("GLFW ERROR NUMBER: " + errno);
    PAG::GUI::getInstance().messageBufferAdd("Description:" + aux);
}

/**
 * Función que se llama cada vez que se tenga que redibujar la ventana
 * @param window
 */
void window_refresh_callback(GLFWwindow *window){
    PAG::Renderer::getInstance().windowRefresh(); //refrescar pantalla
    PAG::GUI::getInstance().render();//Renderizar interfaz
    glfwSwapBuffers(window); //Ultima orden en el callback
    /*
    PAG::GUI::getInstance().messageBufferAdd("REFRESH CALLBACK CALLED");*/
}

/**
 * Función que se llama cada vez que se cambie el area de dibujo del opengl
 * en este caso, solo tenemos un viewport que ocupa toda la pantalla, luego
 * cuando se cambia de tamaño la pantalla entonces se cambia el tamaño del viewport
 * @param window
 * @param width
 * @param height
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    PAG::Renderer::getInstance().viewportResize(width,height);
    PAG::Renderer::getInstance().getViewportSizes(width,height);
}

/**
 * Se llama cada vez que una tecla realiza una acción del teclado
 * @param window
 * @param key
 * @param action
 * @param mods
 */
void key_callback(GLFWwindow *window, int key, int action, int mods){
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS){ //cuando la tecla escape hace la acción presionar
        glfwSetWindowShouldClose(window, GLFW_TRUE);//se cierra a vetana
    }
    //PRACTICA 1
    //Elige el modo según la última tecla que toca

    switch (key) {
        case GLFW_KEY_R:
            mode=0;
            break;
        case GLFW_KEY_G:
            mode=1;
            break;
        case GLFW_KEY_B:
            mode=2;
            break;
        default:
            mode=-1;
            break;
    }

    //Control de camara por teclado
    if(PAG::GUI::getInstance().isControlCameraKey()){
        switch (key) {
            case GLFW_KEY_W:
                PAG::Renderer::getInstance().processKeyCameraMovement(glm::vec3(0,0,0.1));
                break;
            case GLFW_KEY_A:
                PAG::Renderer::getInstance().processKeyCameraMovement(glm::vec3(0.1,0,0));
                break;
            case GLFW_KEY_S:
                PAG::Renderer::getInstance().processKeyCameraMovement(glm::vec3(0,0,-0.1));
                break;
            case GLFW_KEY_D:
                PAG::Renderer::getInstance().processKeyCameraMovement(glm::vec3(-0.1,0,0));
                break;
            case GLFW_KEY_Q:
                PAG::Renderer::getInstance().processKeyCameraMovement(glm::vec3(0,0.1,0));
                break;
            case GLFW_KEY_E:
                PAG::Renderer::getInstance().processKeyCameraMovement(glm::vec3(0,-0.1,0));
                break;
            default:
                break;
        }
    }
}

/**
 * Misma función que la anterior, pero con el ratón
 * @param window
 * @param button
 * @param action
 * @param mods
 */
void mouse_callback(GLFWwindow *window, int button, int action, int mods){

    //cout<<"MOUSE CALLBACK BUTTON: "<<button<<" ACTION: "<< action << std::endl;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        mouseCameraCtr = true;
        glfwGetCursorPos(window,&mousePosX,&mousePosY);

    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
        mouseCameraCtr = false;
    }
}

/**
 * Callback para recuperar la posición del ratón
 * @param window
 * @param xpos
 * @param ypos
 */
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(mouseCameraCtr && PAG::GUI::getInstance().isControlCameraMouse()){ //funciona si está seleccionado

        //Diferencia de posiciones
        double diffX = xpos-mousePosX;
        double diffY = ypos-mousePosY;

        PAG::Renderer::getInstance().processMouseCameraMovement(-diffX,-diffY);

        glfwGetCursorPos(window,&mousePosX,&mousePosY);
    }

}

/**
 * Otra vez lo mismo pero con la rueda
 * @param window
 * @param xoffset
 * @param yoffset
 */
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    //PRACTICA 1
    //Depende de la tecla pulsada cambia un valor u otro
    if (mode!=-1 && PAG::GUI::getInstance().isControlBgColor()){
        PAG::Renderer::getInstance().addBgColor(mode,yoffset);//Suma el yoffset actual para que sea gradual
        glfwSwapBuffers(window);
    }
    if (PAG::GUI::getInstance().isControlZoom()){
        PAG::Renderer::getInstance().processMouseCameraZoom(yoffset);
    }
}


//MAIN
int main()
{ std::cout << "Starting Application PAG - Prueba 02" << std::endl;

    //INICIALIZACIÓN GLFW
    glfwSetErrorCallback((GLFWerrorfun) error_callback);//Callback de errores

    if ( glfwInit () != GLFW_TRUE ) //Inicializar GLFW
    { std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    //Configuración de GLFW
    glfwWindowHint ( GLFW_SAMPLES, 4 ); //Antialiasing
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 1 );

    GLFWwindow *window;//Definir puntero de la ventana
    window = glfwCreateWindow ( width, height, "PAG Introduction", nullptr, nullptr );//Inicializar ventana

    //COmprobar que la ventana ha sido creada
    if ( window == nullptr )
    { std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate ();   //Liberar recrusos de glfw en caso de error
        return -2;
    }

    //Añadir contexto a la ventana
    glfwMakeContextCurrent ( window );

    //INICIALIZACION GLAD

    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    { std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow ( window ); //Destruir ventana
        window = nullptr; //Poner puntero a Null
        glfwTerminate (); //Liberar recrursos de glfw
        return -3;
    }

    //PREPARACIÓN

    //Asignar los callbacks ya definidos
    glfwSetWindowRefreshCallback(window,window_refresh_callback);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetKeyCallback(window, (GLFWkeyfun) key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    //Mostrar contexto ya definido
    PAG::GUI::getInstance().messageBufferAdd(reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
    PAG::GUI::getInstance().messageBufferAdd(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
    PAG::GUI::getInstance().messageBufferAdd(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    PAG::GUI::getInstance().messageBufferAdd(reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    PAG::Renderer::getInstance().rendererInit(); //inicializar opengl
    PAG::Renderer::getInstance().getViewportSizes(width,height); //capturar las dimensiones iniciales

    //PRACTICA 2: Inicializar IMGUI
    PAG::GUI::getInstance().guiInit(window);

    //Crear shader program y cargar modelo

    PAG::Renderer::getInstance().createModelPair("pag09","","",PAG::modelVisualizationTypes::FILL,glm::vec3(1,0,0),glm::vec3(1,0,0),glm::vec3(1,0,0),1);

    //Crear luz ambiente y puntual por defecto
    PAG::Renderer::getInstance().createLight(PAG::lightTypes::AMBIENT,
                                             glm::vec3(0.5,0.5,0.5),
                                             glm::vec3(1,1,1),
                                             glm::vec3(1,1,1),
                                             glm::vec3(1,1,1),
                                             glm::vec3(0,0,0),
                                             1.0f,
                                             1.0f);
    PAG::Renderer::getInstance().createLight(PAG::lightTypes::POINT,
                                             glm::vec3(1,1,1),
                                             glm::vec3(1,1,1),
                                             glm::vec3(1,1,1),
                                             glm::vec3(1,1,1),
                                             glm::vec3(0,0,0),
                                             1.0f,
                                             1.0f);

    //PAG::Renderer::getInstance().getViewportSizes(width,height);

    //CICLO DE EVENTOS
    while ( !glfwWindowShouldClose ( window ) ) //Repetir hasta que se cierre la ventana
    {
        //Camara
        PAG::Renderer::getInstance().processUiCameraMovement(
                PAG::GUI::getInstance().getCameraMovement(),
                PAG::GUI::getInstance().getCameraZoomValue(),
                PAG::GUI::getInstance().getPanAngle(),
                PAG::GUI::getInstance().getTiltAngle(),
                PAG::GUI::getInstance().isDollyForwardPressed(),
                PAG::GUI::getInstance().isDollyBackwardPressed(),
                PAG::GUI::getInstance().isDollyLeftPressed(),
                PAG::GUI::getInstance().isDollyRightPressed(),
                PAG::GUI::getInstance().isCraneUpPressed(),
                PAG::GUI::getInstance().isCraneDownPressed(),
                PAG::GUI::getInstance().getLongitudeAngle(),
                PAG::GUI::getInstance().getLatitudeAngle());

        //Cargar modelos
        if(PAG::GUI::getInstance().getFileBrowserWindow().HasSelected())
        {
            //Añadir cuando el archivo sea .obj
            if (PAG::GUI::getInstance().getFileBrowserWindow().GetSelected().string().substr(PAG::GUI::getInstance().getFileBrowserWindow().GetSelected().string().length()-4)==".obj"){
                //Cargar Shader
                //Crear Shader del texto
                if(PAG::GUI::getInstance().getShaderLoadInputText()!=""){
                    try{
                        //Crear modelo y ShaderProgram
                        //mensaje de error de textura
                        if(PAG::GUI::getInstance().getModelVisualizationType()==PAG::modelVisualizationTypes::TEXTURED
                           &&PAG::GUI::getInstance().getTextureLoadInputText()==""){
                            PAG::GUI::getInstance().messageBufferAdd("ERROR: No texture selected.");
                        }

                        if((PAG::GUI::getInstance().getModelVisualizationType()==PAG::modelVisualizationTypes::TEXTURED
                            &&PAG::GUI::getInstance().getTextureLoadInputText()!="")
                           ||(PAG::GUI::getInstance().getModelVisualizationType()!=PAG::modelVisualizationTypes::TEXTURED)){
                            PAG::Renderer::getInstance().createModelPair(PAG::GUI::getInstance().getShaderLoadInputText(),
                                                                         PAG::GUI::getInstance().getFileBrowserWindow().GetSelected().string(),
                                                                     PAG::GUI::getInstance().getTextureLoadInputText(),
                                                                     PAG::GUI::getInstance().getModelVisualizationType(),
                                                                     glm::vec3 (PAG::GUI::getInstance().getModelAmbientColor()[0],
                                                                                PAG::GUI::getInstance().getModelAmbientColor()[1],
                                                                                PAG::GUI::getInstance().getModelAmbientColor()[2]),
                                                                     glm::vec3 (PAG::GUI::getInstance().getModelDiffuseColor()[0],
                                                                                PAG::GUI::getInstance().getModelDiffuseColor()[1],
                                                                                PAG::GUI::getInstance().getModelDiffuseColor()[2]),
                                                                     glm::vec3 (PAG::GUI::getInstance().getModelSpecularColor()[0],
                                                                                PAG::GUI::getInstance().getModelSpecularColor()[1],
                                                                                PAG::GUI::getInstance().getModelSpecularColor()[2]),
                                                                     PAG::GUI::getInstance().getModelSpecularExponent());

                        }
                    }catch (const std::exception& e){ //capturar excepción en caso de error
                        PAG::GUI::getInstance().messageBufferAdd(e.what());
                    }
                } else {
                    PAG::GUI::getInstance().messageBufferAdd("ERROR: No shader selected.");
                }
            }
            else
            {
                PAG::GUI::getInstance().messageBufferAdd("ERROR: Unsupported model format.");
            }
            PAG::GUI::getInstance().clearModelLoader();
        }

        //Transform
        if (PAG::GUI::getInstance().isModelTransformApplyPressed()){
            PAG::Renderer::getInstance().processUiModelTransform(
                    PAG::GUI::getInstance().getCurrentModelIndex(),
                    PAG::GUI::getInstance().getModelTransform(),
                    PAG::GUI::getInstance().getModelTranslate(),
                    PAG::GUI::getInstance().getModelRotateAxis(),
                    PAG::GUI::getInstance().getModelRotateAngle(),
                    PAG::GUI::getInstance().getModelScale());
        }
        if(PAG::GUI::getInstance().isModelMaterialApplyPressed()){
            PAG::Renderer::getInstance().processUiModelMaterial(
                    PAG::GUI::getInstance().getCurrentModelIndex(),
                    glm::vec3(PAG::GUI::getInstance().getModelAmbientColorTransform()[0],
                              PAG::GUI::getInstance().getModelAmbientColorTransform()[1],
                              PAG::GUI::getInstance().getModelAmbientColorTransform()[2]),
                    glm::vec3(PAG::GUI::getInstance().getModelDiffuseColorTransform()[0],
                              PAG::GUI::getInstance().getModelDiffuseColorTransform()[1],
                              PAG::GUI::getInstance().getModelDiffuseColorTransform()[2]),
                    glm::vec3(PAG::GUI::getInstance().getModelSpecularColorTransform()[0],
                              PAG::GUI::getInstance().getModelSpecularColorTransform()[1],
                              PAG::GUI::getInstance().getModelSpecularColorTransform()[2]),
                    PAG::GUI::getInstance().getModelSpecularExponentTransform(),
                    PAG::GUI::getInstance().getEditModelVisualizationType());

            if(PAG::GUI::getInstance().getEditModelVisualizationType()==PAG::modelVisualizationTypes::TEXTURED){
                if (PAG::GUI::getInstance().getTextureEditLoadInputText() !=""){
                    try {
                        PAG::Renderer::getInstance().loadTexture(PAG::GUI::getInstance().getTextureEditLoadInputText(),
                                                                 PAG::GUI::getInstance().getCurrentModelIndex()); //Añadir textura al último
                    } catch (const std::exception& e) {
                        PAG::GUI::getInstance().messageBufferAdd(e.what());
                    }
                } else{
                    PAG::GUI::getInstance().messageBufferAdd("ERROR: No texture selected.");
                }
            }
        }

        //Borrar Modelos
        if(PAG::GUI::getInstance().isModelDeletePressed()){
            PAG::Renderer::getInstance().deleteModel(PAG::GUI::getInstance().getCurrentModelIndex());
            if(!PAG::Renderer::getInstance().getModelList()->empty()){
                if((PAG::GUI::getInstance().getCurrentModelIndex()==PAG::Renderer::getInstance().getModelList()->size())){
                    PAG::GUI::getInstance().setCurrentModelIndex(PAG::GUI::getInstance().getCurrentModelIndex()-1);
                }
            }
        }

        //Luces
        if(PAG::GUI::getInstance().isCreateLightPressed()){
            PAG::Renderer::getInstance().createLight(PAG::GUI::getInstance().getCreateLightType(),
                                                     glm::vec3(PAG::GUI::getInstance().getLightAmbientColor()[0],
                                                                PAG::GUI::getInstance().getLightAmbientColor()[1],
                                                               PAG::GUI::getInstance().getLightAmbientColor()[2]),
                                                     glm::vec3(PAG::GUI::getInstance().getLightSpecularColor()[0],
                                                               PAG::GUI::getInstance().getLightSpecularColor()[1],
                                                               PAG::GUI::getInstance().getLightSpecularColor()[2]),
                                                     glm::vec3(PAG::GUI::getInstance().getLightDiffuseColor()[0],
                                                               PAG::GUI::getInstance().getLightDiffuseColor()[1],
                                                               PAG::GUI::getInstance().getLightDiffuseColor()[2]),
                                                     PAG::GUI::getInstance().getLightPosition(),
                                                     PAG::GUI::getInstance().getLightDirection(),
                                                     PAG::GUI::getInstance().getLightGamma(),
                                                     PAG::GUI::getInstance().getLightS());
        }

        if(PAG::GUI::getInstance().isEditLightPressed()){
            PAG::Renderer::getInstance().processUiLightEdit(PAG::GUI::getInstance().getCurrentLightIndex(),
                                                     glm::vec3(PAG::GUI::getInstance().getLightAmbientColorEdit()[0],
                                                               PAG::GUI::getInstance().getLightAmbientColorEdit()[1],
                                                               PAG::GUI::getInstance().getLightAmbientColorEdit()[2]),
                                                     glm::vec3(PAG::GUI::getInstance().getLightSpecularColorEdit()[0],
                                                               PAG::GUI::getInstance().getLightSpecularColorEdit()[1],
                                                               PAG::GUI::getInstance().getLightSpecularColorEdit()[2]),
                                                     glm::vec3(PAG::GUI::getInstance().getLightDiffuseColorEdit()[0],
                                                               PAG::GUI::getInstance().getLightDiffuseColorEdit()[1],
                                                               PAG::GUI::getInstance().getLightDiffuseColorEdit()[2]),
                                                     PAG::GUI::getInstance().getLightPositionEdit(),
                                                     PAG::GUI::getInstance().getLightDirectionEdit(),
                                                     PAG::GUI::getInstance().getLightGammaEdit(),
                                                     PAG::GUI::getInstance().getLightSEdit());
        }

        if(PAG::GUI::getInstance().isDeleteLightPressed()){
            PAG::Renderer::getInstance().deleteLight(PAG::GUI::getInstance().getCurrentLightIndex());
            if(!PAG::Renderer::getInstance().getLightList()->empty()){
                if((PAG::GUI::getInstance().getCurrentLightIndex()==PAG::Renderer::getInstance().getLightList()->size())){
                    PAG::GUI::getInstance().setCurrentLightIndex(PAG::GUI::getInstance().getCurrentLightIndex()-1);
                }
            }
        }

        PAG::Renderer::getInstance().windowRefresh(); //Refrescar ventana constantemente

        //Dibujar interfaz
        PAG::GUI::getInstance().newFrame();//Llamadas de la interfaz de usuario
        PAG::GUI::getInstance().drawMessageWindow(10,10,1.0f);
        PAG::GUI::getInstance().drawControlWindow(10,200,1.0f,
                                                  PAG::Renderer::getInstance().getModelList(),
                                                  PAG::Renderer::getInstance().getLightList(),
                                                  reinterpret_cast<float *>(PAG::Renderer::getInstance().getBgColor()),
                                                  PAG::Renderer::getInstance().getLightList()->size());
        PAG::Renderer::getInstance().updateBgColor(); //Poner esto aquí para la función de cambiar color de fondo

        //Dibujar escena
        PAG::GUI::getInstance().render();//Renderizar interfaz
        glfwSwapBuffers(window);
        glfwPollEvents(); //Despachar pila de eventos
    }

    //TERMINAR APLICACIÓN

    std::cout << "Finishing application pag prueba" << std::endl;

    //Terminar imgui
    PAG::GUI::getInstance().shutDown();

    glfwDestroyWindow ( window ); //Destruir ventana
    window = nullptr; //Poner puntero a nulo
    glfwTerminate (); //Liberar recursos de GLFW
}

// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW


#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Utilities/Renderer.h"
#include "Utilities/GUI.h"

//PRÁCTICA 1: variable global del modo de cambiar de fondo
//int mode = -1; //Modo, rojo, verde, azul
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
    /*
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
    }*/
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
    if(mouseCameraCtr){

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
    /*
    buffer << "SCROLL CALLBACK X POS: " << xoffset
              << "  Y POS: " << yoffset << std::endl;
    //PRACTICA 1
    //Depende de la tecla pulsada cambia un valor u otro
    /*if (mode!=-1){
        PAG::Renderer::getInstance().addBgColor(mode,yoffset);//Suma el yoffset actual para que sea gradual
        glfwSwapBuffers(window);
    }*/
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
    try{
        PAG::Renderer::getInstance().createShaderProgram("pag03");
    }
    catch (const std::exception& e){ //capturar excepción en caso de error
        PAG::GUI::getInstance().messageBufferAdd( e.what());
    }

    PAG::Renderer::getInstance().createModel();

    //CICLO DE EVENTOS
    while ( !glfwWindowShouldClose ( window ) ) //Repetir hasta que se cierre la ventana
    {

        //El renderer captura la info de GUI desde el main para hacer el movimiento de la cámara
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

        PAG::Renderer::getInstance().windowRefresh(); //Refrescar ventana constantemente
        PAG::GUI::getInstance().newFrame();//Llamadas de la interfaz de usuario

        //Dibujar interfaz
        PAG::GUI::getInstance().drawMessage(10,10,1.0f,"Mensajes",PAG::GUI::getInstance().getMessageBufferText().c_str());

        PAG::GUI::getInstance().drawColorWheel(500, 10, 1.0f,
                                               reinterpret_cast<float *>(PAG::Renderer::getInstance().getBgColor()), "Fondo", "Actual");
        PAG::Renderer::getInstance().updateBgColor();
        PAG::GUI::getInstance().drawShaderLoadWindow(10,450,1.0f,"Shader load");
        PAG::GUI::getInstance().drawCameraControls(500, 450,1.0f,"Camera");

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

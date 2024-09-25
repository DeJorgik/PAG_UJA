// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
//#include "imgui/imgui_impl_opengl3_loader.h"

#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Utilities/Renderer.h"

//PRÁCTICA 1: variable global del modo de cambiar de fondo
int mode = -1; //Modo, rojo, verde, azul
//PRÁCTICA 2 buffer con el texto que aparecerá por patalla
std::stringstream buffer;

/**
 * Función que muestra por pantalla los errores que ocurren
 * @param _errno
 * @param desc
 */
void error_callback(int errno, const char* desc){
    std::string aux(desc);
   buffer << "GLFW ERROR NUMBER: "<< errno << ":" << aux << std::endl;
}

/**
 * Función que se llama cada vez que se tenga que redibujar la ventana
 * @param window
 */
void window_refresh_callback(GLFWwindow *window){
    //PAG::Renderer::getInstance().windowRefresh(); //refrescar pantalla
    glfwSwapBuffers(window); //Ultima orden en el callback
    buffer << "REFRESH CALLBACK CALLED" << std::endl;
}

/**
 * Función que se llama cada cez que se cambie el area de dibujo del opengl
 * en este caso, solo tenemos un viewport que ocupa toda la pantalla, luego
 * cuando se cambia de tamaño la pantalla entonces se cambia el tamaño del viewport
 * @param window
 * @param width
 * @param height
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    PAG::Renderer::getInstance().viewportResize(width,height);
    buffer << "RESIZE CALLBACK CALLED" << std::endl;
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
}

/**
 * Misma función que la anterior, pero con el ratón
 * @param window
 * @param button
 * @param action
 * @param mods
 */
void mouse_callback(GLFWwindow *window, int button, int action, int mods){
    buffer<<"MOUSE CALLBACK BUTTON: "<<button<<" ACTION: "<< action << std::endl;
}

/**
 * Otra vez lo mismo pero con la rueda
 * @param window
 * @param xoffset
 * @param yoffset
 */
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    buffer << "SCROLL CALLBACK X POS: " << xoffset
              << "  Y POS: " << yoffset << std::endl;
    //PRACTICA 1
    //Depende de la tecla pulsada cambia un valor u otro
    if (mode!=-1){
        PAG::Renderer::getInstance().addBgColor(mode,yoffset);//Suma el yoffset actual para que sea gradual
        glfwSwapBuffers(window);
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
    window = glfwCreateWindow ( 1024, 576, "PAG Introduction", nullptr, nullptr );//Inicializar ventana

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

    //Mostrar contexto ya definido
    std::cout << glGetString ( GL_RENDERER ) << std::endl
              << glGetString ( GL_VENDOR ) << std::endl
              << glGetString ( GL_VERSION ) << std::endl
              << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

    PAG::Renderer::getInstance().rendererInit(); //inicializar opengl

    //PRACTICA 2: Inicializar IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window,true); //Inicializar para opengl
    ImGui_ImplOpenGL3_Init();

    //CICLO DE EVENTOS

    while ( !glfwWindowShouldClose ( window ) ) //Repetir hasta que se cierre la ventana
    {
        PAG::Renderer::getInstance().windowRefresh(); //Refrescar ventana constantemente

        //Llamadas de la interfaz de usuario
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //Dibujar interfaz

        //Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2 (10, 10), ImGuiCond_Once );
        if ( ImGui::Begin ( "Mensajes" )){
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario
            ImGui::TextUnformatted(buffer.str().c_str()); //texto es salida del buffer
            //resetear si llega al maximo
            if(buffer.str().size()==buffer.str().max_size()){
                buffer.str(std::string());
            }
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();

        ImGui::SetNextWindowPos ( ImVec2 (200, 10), ImGuiCond_Once );
        if (ImGui::Begin("Fondo")){
            ImGui::SetWindowFontScale ( 1.0f );
            //Le paso un puntero al color picker
            ImGui::ColorPicker3("Actual",PAG::Renderer::getInstance().getBgColor(),ImGuiColorEditFlags_PickerHueWheel);
            PAG::Renderer::getInstance().updateBgColor(); //Cambia el color del fondo (ver si se puede hacer con callbacks)
        }
        ImGui::End();

        //Dibujar escena

        //Renderizar interfaz
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents(); //Despachar pila de eventos
    }

    //TERMINAR APLICACIÓN

    std::cout << "Finishing application pag prueba" << std::endl;

    //Terminar imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext ();

    glfwDestroyWindow ( window ); //Destruir ventana
    window = nullptr; //Poner puntero a nulo
    glfwTerminate (); //Liberar recursos de GLFW
}

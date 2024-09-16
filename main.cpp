#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//FUNCIONES MIAS QUE LUEGO SE VAN A USAR EN EL BUCLE DE EJECUCIÓN
//ESTO ES COMO EN GRAFICAS DE TERCERO, SON FUNCIONES QUE DEFINO YO
//QUE LUEGO SE AÑADEN PARA SER EJECUTADAS CUANDO SEA NECESARIO

/**
 * función que se llama en el bucle
 * muestra por pantalla los errores que ocurren
 * @param _errno
 * @param desc
 */
void error_callback(int errno, const char* desc){
    std::string aux(desc);
    std::cout << "GLFW ERROR NUMBER: "<< errno << ":" << aux << std::endl;
}

/**
 * función que se llama cada vez que se tenga que redibujar la ventana
 * @param window
 */
void window_refresh_callback(GLFWwindow *window){
    // - Borra los buffers (color y profundidad)
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (en el que se ha estado dibujando) por el
    // que se mostraba hasta ahora (front).
    glfwSwapBuffers(window); //OJO TIENE QUE SER LA ULTIMA ORDEN EN EL CALLBACK
    std::cout << "REFRESH CALLBACK CALLED" << std::endl;
}

/**
 * función que se llama cada cez que se cambie el area d dibujo del opengl
 * en este caso, solo tenemos un viewport que ocupa toda la pantalla, luego
 * cuando se cambia de tamaño la pantalla entonces se cambia el tamaño del viewport
 * @param window
 * @param width
 * @param height
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0,0,width,height); //pone desde donde va hasta donde va
                                        //en este caso el viewport equivale a toda la ventana
    std::cout << "RESIZE CALLBACK CALLED" << std::endl;
}

/**
 * Se llama cada vez que una tecla realiza una acción
 * @param window
 * @param key
 * @param action
 * @param mods
 */
void key_callback(GLFWwindow *window, int key, int action, int mods){
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS){ //cuando la tecla escape hace la acción presionar
        glfwSetWindowShouldClose(window, GLFW_TRUE);//se cierra a vetana
    }
    std::cout<<"KEY CALLBACK CALLED"<<std::endl;
}

/**
 * Misma movida que el anterior, pero con el ratón
 * @param window
 * @param button
 * @param action
 * @param mods
 */
void mouse_callback(GLFWwindow *window, int button, int action, int mods){
    std::cout<<"MOUSE CALLBACK BUTTON: "<<button<<" ACTION: "<< action << std::endl;
}

/**
 * otra vez lo mismo pero con la rueda
 * @param window
 * @param xoffset
 * @param yoffset
 */
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    std::cout << "SCROLL CALLBACK X POS: " << xoffset
              << "  Y POS: " << yoffset << std::endl;
}

//MAIN
int main()
{ std::cout << "Starting Application PAG - Prueba 01" << std::endl;

    //INICIALIZACIÓN

    //ANTES DEL INIT HAY QUE PONER EL CALLBACK DE LOS ERRORES
    glfwSetErrorCallback((GLFWerrorfun) error_callback);

    //PONER GLFW INIT
    //poner lo de que si no es true saca un mensaje de error y termina
    if ( glfwInit () != GLFW_TRUE )
    { std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    //CONFIGURACIÓN DE GLFW (CONTEXTO OPENGL) ANTES DE ABRIR LA VENTANA
    //EL GLFW SAMPLES ES EL NUMERO DE PIXELES QUE UTILIZA DE MUESTRA PARA ANTIALIASING
    //MAS ALTO = MENOS DIENTES DE SIERRA
    glfwWindowHint ( GLFW_SAMPLES, 4 ); // - Activa antialiasing con 4 muestras.
    //ESTO DE AQUI SON MOVIDAS QUE NO ENTIENDO PERO QUE SON NECESARIAS
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // - Esta y las 2
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // siguientes activan un contexto
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 1 ); // OpenGL Core Profile 4.1.

    //DEFIICIÓN DEL PUNTERO DONDE SE GUARDA LA VENTANA DE LA APLICACIÓN
    GLFWwindow *window;

    //INICIALIZACIÓN DE LA VENTANA
    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.
    window = glfwCreateWindow ( 1024, 576, "PAG Introduction", nullptr, nullptr );

    //SE COMPRUEBA QUE LA VENTANA EXISTE(NO ES NULA)
    // - Comprobamos si la creación de la ventana ha tenido éxito.
    if ( window == nullptr )
    { std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate ();   //IMPORTANTE: PARA CANCELAR O SALIR HAY QUE PONER ESTO
                            //PARA QUE SE LIBEREN LOS LOS RECURSOS DE GLFW
        return -2;
    }

    //DESPUES DE CREAR LA VENTANA, AÑADIMOS EL CONTEXTO QUE HEMOS DEFINIDO ANTES
    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );

    //INICIALIZACIÓN GLFW
    //1.- GLFWINIT
    //2.- CONFIGURACIÓN GLFW/CONTEXTO
    //3.- DEFINIR PUNTERO VENTANA
    //4.- INICIALIZAR VENTANA
    //5.- COMPROBAR QUE LA VENTANA EXISTE
    //6.- ASOCIAR CONTEXTO A LA VENTANA

    //INICIALIZAR GLAD
    //Poner caso en el que no funcioe
    // - Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    { std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow ( window ); //PRIMERO DESTRUYE LA VENTANA (ANTES NO LO HACE PQ LA VENTANA NO EXISTE AUN)
        window = nullptr; //NOS ASEGURAMOS Y LA PONEMOS A NULL
        glfwTerminate (); //HACER EL TERMINATE PARA LIBERAR GLFW
        //1.- DESTRUIR VENTANA
        //2.- VENTANA NULL
        //3.- GLFW TERMINTE
        return -3;
    }

    //COSAS QUE SE EJECUTAN SOLO AL PRINCIPIO

    //MOSTRAR PROPIEDADES DEL CONTEXTO YA CREADO
    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    std::cout << glGetString ( GL_RENDERER ) << std::endl
            << glGetString ( GL_VENDOR ) << std::endl
            << glGetString ( GL_VERSION ) << std::endl
            << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

    //ANTES DE ESTO PONEMOS LOS CALLBACKS QUE HEMOS DEFINIDO ANTES+
    glfwSetWindowRefreshCallback(window,window_refresh_callback);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetKeyCallback(window, (GLFWkeyfun) key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //GLCLEARCOLOR PONE EL "COLOR DE FONDO" DE LA VENTANA
    // - Establecemos un gris medio como color con el que se borrará el frame buffer.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glClearColor ( 0.6, 0.6, 0.6, 1.0 );

    //PONER PROFUNDIDAD
    // - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glEnable ( GL_DEPTH_TEST );

    //COSAS QUE SE EJECUTAN CONSTANTEMENTE

    //CICLO DE EVENTOS, ESTO SE REPITE HASTA QUE SE CIERRE LA VENTANA
    // - Ciclo de eventos de la aplicación. La condición de parada es que la
    // ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose ( window ) )
    {
    glfwPollEvents ();
    }

    //TERMINAR Y LIBERAR RECURSOS (IGUAL QUE LO DE ANTES)
    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
    glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
}
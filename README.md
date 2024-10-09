# PAG UJA 2024/25
## Jorge Robles Calvo 77397648P jrc00083
Practicas de Programación de aplicaciones gráficas en la UJA.

## PRÁCTICA 1:
Se pueden controlar los valores de RGB del color de fondo con la rueda del ratón. Después de pulsar las teclas R, G o B si se mueve hacia delante o hacia abajo la rueda del ratón se alterarán los valores respectivos a la última tecla que se pulsó.

## PRÁCTICA 2:
Ahora, utilizando la librería de ImGui, aparecen dos ventanas emergentes en la aplicación. Una muestra los mensajes de los callbacks, los cuales antes aparecían por consola. La otra muestra una rueda de color con la que se puede alterar el color del fondo. La funcionalidad de la práctica 1 sigue estando.

### Clases utilizadas

Las funciones de **OpenGL** han sido encapsuladas en la clase **PAG::Renderer.** De al misma forma, las funciones de **Dear ImGui** que se utilizarán en la aplicación han sido encapsuladas en la clase **PAG::GUI.** Ambas clases siguen el patrón de diseño Singleton. De esta forma, en el main sólo se hacen llamadas a estas clases, a GLFW y a GLAD.

![img.png](Resources/img.png)

_La aplicación sigue la misma estructura que la estructura propuesta en los guiones de prácticas._

## PRÁCTICA 3:

La clase **PAG::Renderer** contiene ahora dos métodos para la creación de un shader program y un modelo, siendo estos **createShaderProgram()** y **createModel()** respectivamente. El modelo en cuestión es un triángulo equilátero donde cada uno de los vértices tiene el color verde, rojo y azul, siendo el color de cada fragmento una aproximación calculada respecto a la distancia de cada uno. El método **createShaderProgram()** utiliza la función privada **loadShader(shaderLocation)** que toma
 como entrada la localización del shader para cargarlo. En caso de error a la hora de crear tanto el shader program como los shaders, se lanza una excepción que se muetra por la pantalla de mensajes definida en la práctica anterior. Las funciones privadas **shaderCompileErrorExceptionLaucher(GLuint shaderId)** y **shaderProgramCompileErrorExceptionLauncher(GLuint shaderProgramId)** funcionan como medidas de seguridad que lanzan excepciones con los mensajes de error de la compilación de los shaders y el shader program. Tanto el vertex shader como el fragment shader se encuentran en la carpeta *Shaders*.

El proyecto cuenta con dos **VBOs no entrelazados** para la posición y el color y otro **VBO entrelazado** comentado que contiene ambos atributos.

#### Pregunta: Si redimensionas la ventana de la aplicación, verás que el triángulo no permanece igual, sino que se deforma al mismo tiempo que la ventana. ¿A qué crees que se  debe este comportamiento?

Se debe a que al redimensionar la ventana no estamos teniendo en cuenta el *aspect ratio*. Cuando redimensionamos la ventana, no alteramos el aspecto de la cámara para que refleje los cambios, dando como resultado una imagen deformada cuando se estira mucho la ventana. Si el aspecto fuera adaptativo y se correspondiera con el ancho de la ventana entre el alto, la imagen no se deformaría.

## PRÁCTICA 4:

Para el desacoplamiento de los shaders se utilizan dos nuevas clases: **PAG::ShaderProgram** y **ShaderObject.** PAG::ShaderProgram encapsula las funciones necesarias para crear y enlazar un shader program. Cuenta con las funciones definidas en la práctica de error para el lanzamiento de excepciones en caso de errores. Además cuenta con dos punteros a dos objetos **ShaderObject** que corresponden al vertex shader y el fragment shader del programa. PAG::ShaderObject encapsula las funciones necesarias para crear un shader, ya sea un vertex shader o un fragment shader. Para hacer esta distinción se utiliza un tipo enumerado **shaderType** cuyos valores son **VERTEX** y **FRAGMENT.**

### Generar Shader Program

Para poder generar un shader program, la clase PAG::Renderer cuenta con un puntero a un objeto tipo PAG::ShaderProgram el cual instancia cada vez que se quiere crear un nuevo shader program. Después PAG::Renderer llama a la función createShaderProgram de este objeto, pasándole como atributo un string con la parte común de los archivos de los shaders, en este caso si los archivos son "pag03-vs.glsl" y "pag03-fs.glsl" sólo se le pasa la cadena "pag03".
Entonces la clase PAG::ShaderProgram instancia dos shader programs, uno con el valor **VERTEX** en el atributo *type* y el otro con el valor **FRAGMENT**. Según el tipo de shader, cada ShaderObject crea su shader cargando su código glsl correspondiente utilizando la cadena de texto común ya proporcionada. Cuando ambos shaders están creados, PAG::Shader program recibe los ids de estos utilizando la función *getId* de PAG::Shader Object y enlaza el shader program.
Finalmente, PAG::Renderer recibe el id del shader program mediante la función *getIdSp* para utilizarlo en la aplicación.

### Cargar Shader Program mediante interfaz

La clase PAG::GUI contiene una función *drawShaderLoadWindow* que genera una ventana con una entrada de texto y un botón. PAG::Renderer recibe el estado del botón llamando a la función *isShaderLoadButtonPressed* de PAG::GUI, cuando el botón se presiona, recibe el texto de la entrada con la función *getShaderLoadInputText* de PAG::GUI. Si el texto no es vacío, PAG::Renderer destruye la instancia del shader program, el cual destruye sus dos shader objects. Después, crea un nuevo shader program utilizando el texto de entrada como la parte común de los shaders tal y como se explica en el apartado anterior. 

### UML

````plantuml
class OpenGL
class ImGui
class PAG::Renderer{
PAG::ShaderProgram* shaderProgram
}
class PAG::GUI
class PAG::ShaderProgram{
PAG::ShaderObject* vertexShader
PAG::ShaderObject* fragmentShader
}
class PAG::ShaderObject

PAG::Renderer-->PAG::ShaderProgram:instancia
PAG::Renderer-->OpenGL:llama
PAG::Renderer->PAG::GUI:llama
PAG::GUI-->ImGui:llama
PAG::ShaderProgram-->PAG::ShaderObject:instancia
PAG::ShaderProgram->OpenGL:utiliza
PAG::ShaderObject->OpenGL:utiliza

````
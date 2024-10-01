# PAG UJA 2024/25
## Jorge Robles Calvo 77397648P jrc00083
Practicas de Programación de aplicaciones gráficas en la UJA.

## PRÁCTICA 1:
Se pueden controlar los valores de RGB del color de fondo con la rueda del ratón. Después de pulsar las teclas R, G o B si se mueve hacia delante o hacia abajo la rueda del ratón se alterarán los valores respectivos a la última tecla que se pulsó.

## PRÁCTICA 2:
Ahora, utilizando la librería de ImGui, aparecen dos ventanas emergentes en la aplicación. Una muestra los mensajes de los callbacks, los cuales antes aparecían por consola. La otra muestra una rueda de color con la que se puede alterar el color del fondo. La funcionalidad de la práctica 1 sigue estando.

### Clases utilizadas

Las funciones de **OpenGL** han sido encapsuladas en la clase **PAG::Renderer.** De al misma forma, las funciones de **Dear ImGui** que se utilizarán en la aplicación han sido encapsuladas en la clase **PAG::GUI.** Ambas clases siguen el patrón de diseño Singleton. De esta forma, en el main sólo se hacen llamadas a estas clases, a GLFW y a GLEW.

![img.png](Resources/img.png)

_La aplicación sigue la misma estructura que la estructura propuesta en los guiones de prácticas._

## PRÁCTICA 3:

La clase **PAG::Renderer** contiene ahora dos métodos para la creación de un shader program y un modelo, siendo estos **createShaderProgram()** y **createModel()** respectivamente. El modelo en cuestión es un triángulo equilátero donde cada uno de los vértices tiene el color verde, rojo y azul, siendo el color de cada fragmento una aproximación calculada respecto a la distancia de cada uno. El método **createShaderProgram()** utiliza la función privada **loadShader(shaderLocation)** que toma
 como entrada la localización del shader para cargarlo. En caso de error a la hora de crear tanto el shader program como los shaders, se lanza una excepción que se muetra por la pantalla de mensajes definida en la práctica anterior. Las funciones privadas **shaderCompileErrorExceptionLaucher(GLuint shaderId)** y **shaderProgramCompileErrorExceptionLauncher(GLuint shaderProgramId)** funcionan como medidas de seguridad que lanzan excepciones con los mensajes de error de la compilación de los shaders y el shader program. Tanto el vertex shader como el fragment shader se encuentran en la carpeta *Shaders*.

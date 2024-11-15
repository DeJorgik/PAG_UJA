#version 410
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
uniform mat4 mModelViewProj;

//Bloque de salida
out outputVS
{   vec3 posV;
    vec3 normalV; //no usamos la normal aún
} output;

void main (){
    output.posV = mModelViewProj * vec4 ( pos, 1 );
    gl_Position = output.posV
}
#version 410
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
uniform mat4 mModelViewProj;
void main (){
    gl_Position = mModelViewProj * vec4 ( pos, 1 );
}
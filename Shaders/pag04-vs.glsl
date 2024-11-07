#version 410
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
uniform mat4 mModelViewProj;
out vec3 vertexColor;
void main (){
    gl_Position = mModelViewProj * vec4 ( pos, 1 );
    vertexColor = vec3(1,0,0) + color;
}
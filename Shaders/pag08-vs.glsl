#version 410
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
uniform mat4 mModelViewProj; //matriz modelado visión y proyección
uniform mat4 mModelView; //matriz modelado y visión
//le mando la posición y la normal al fs después de nultiplicarlo por mv
out vec3 pos;
out vec3 normal;
void main () {
    normal = vec3(mModelView*vec4(vNormal,0.0)); 
    pos = vec3(mModelView*vec4(vPos,1.0));
    gl_Position = mModelViewProj * vec4 ( vPos, 1.0 );
}

#version 410
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextureCoordinate;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

uniform mat4 mModelViewProj; //matriz modelado visión y proyección
uniform mat4 mModelView; //matriz modelado y visión

uniform mat4 mModelViewIT; //inversa de las traspuesta de la matriz de modelado y visión
uniform vec3 vLightPos; //OJO: LA POSICIÓN Y LA DIRECCIÓN ESTÁN EN EL ESPACIO DE VISIÓN !!!
uniform vec3 vLightDir;

//le mando la posición y la normal al fs después de nultiplicarlo por mv
out vec3 pos;
out vec3 normal;
out vec2 textureCoordinate;

//le mando tmb los datos para el normal mapping
out vec3 posTg;
out vec3 lightPosTg;
out vec3 lightDirTg;

void main () {
    //Calculos normales
    normal = vec3(mModelView*vec4(vNormal,0.0));
    pos = vec3(mModelView*vec4(vPos,1.0));
    textureCoordinate = vTextureCoordinate;
    //Calculos para normal mapping
    vec3 normalIT = normalize(vec3(mModelViewIT*vec4(vNormal,0.0)));
    vec3 tangent = normalize(vec3(mModelViewIT*vec4(vTangent,0.0)));
    vec3 bitangent = normalize(cross(normalIT,vTangent));
    mat3 TBN = transpose(mat3(tangent,bitangent,normalIT));
    posTg = TBN*vec3(mModelView*vec4(vPos,1.0));
    lightPosTg = TBN*vLightPos;
    lightDirTg = TBN*vLightDir;
    //Posición del vértice
    gl_Position = mModelViewProj * vec4 ( vPos, 1.0 );
}

// FRAGMENT SHADER
#version 410
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;

out vec4 fragColor;
in vec3 normal;

subroutine vec4 getColor();
subroutine uniform getColor getColorMethod;

subroutine ( getColor )
vec4 colorMaterial ()
{ return vec4 ( matAmbient, 1 );
}
subroutine ( getColor )
vec4 colorWireframe ()
{ return vec4 ( 0, 0, 0, 1 );
}

void main ()
{ fragColor = getColorMethod();
}
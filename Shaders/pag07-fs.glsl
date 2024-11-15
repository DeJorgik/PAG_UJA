// FRAGMENT SHADER
#version 410
out vec4 fragColor;
subroutine vec4 getColor();
subroutine uniform getColor getColorMethod;

subroutine ( getColor )
vec4 colorMaterial ()
{ return vec4 ( 1, 0, 0, 1 );
}
subroutine ( getColor )
vec4 colorWireframe ()
{ return vec4 ( 0, 1, 0, 1 );
}

//Bloque de entrada
in outputVS
{   vec3 posV;
    vec3 normalV;
} input;


void main ()
{ fragColor = getColorMethod ();
}
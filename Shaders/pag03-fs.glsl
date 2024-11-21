#version 410
in vec3 vertexColor;
out vec4 fragColor;

subroutine vec4 getColor();
subroutine uniform getColor getColorMethod;

subroutine ( getColor )
vec4 colorMaterial ()
{ return vec4 ( vertexColor, 1.0 );
}
subroutine ( getColor )
vec4 colorWireframe ()
{ return vec4 ( 0, 0, 0, 1 );
}

void main ()
{ fragColor = getColorMethod();
}
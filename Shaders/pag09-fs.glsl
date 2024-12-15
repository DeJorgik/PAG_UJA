// FRAGMENT SHADER
#version 410

//Pongo aquí esto para no calcular pi cada vez que lo necesite
#define PI radians(180)

//Datos VS
in vec3 pos;
in vec3 normal;
in vec2 textureCoordinate;

//Reflexión Material
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float exponent; //EL EXPONENTE SPECULAR ES DEL MATERIAL

//Textura
uniform sampler2D sampler;

//Valores iluminación
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform float spotAngle;

out vec4 fragColor;


//Una subrutina con dos implementaciones que devuelve el color de la superficie
//puede ser la del material o la de la textura
subroutine vec3 getDiffuseColor();
subroutine uniform getDiffuseColor getDiffuseColorMethod;

subroutine ( getDiffuseColor )
vec3 materialColor()
{ return Kd;
}

subroutine ( getDiffuseColor )
vec3 textureColor()
{ return texture(sampler,textureCoordinate).rgb;
}

//Una subrutina para multiplicar cada tipo de luz
//una implementación extra para el modo wireframe
subroutine vec4 getColor();
subroutine uniform getColor getColorMethod;

subroutine ( getColor )
vec4 ambientLight()
{ return vec4 ( Ia*Ka, 1.0);
}

subroutine ( getColor )
vec4 pointLight ()
{ 
  vec3 n = normalize (normal);
  vec3 l = normalize (lightPos-pos);
  vec3 v = normalize (-pos);
  vec3 r = reflect (-l,n);

  vec3 diffuse = (Id*getDiffuseColorMethod()*max(dot(l,n),0.0));
  vec3 specular = (Is*Ks*pow(max(dot(r,v),0.0),exponent));
  return vec4 (diffuse+specular, 1.0);
}

subroutine ( getColor )
vec4 directionLight ()
{
  vec3 n = normalize (normal);
  vec3 l = -lightDir;
  vec3 v = normalize (-pos);
  vec3 r = reflect (-l,n);

  vec3 diffuse = (Id*getDiffuseColorMethod()*max(dot(l,n),0.0));
  vec3 specular = (Is*Ks*pow(max(dot(r,v),0.0),exponent));
  return vec4 ( diffuse+specular, 1.0 );
}

subroutine ( getColor )
vec4 spotLight ()
{ 
  vec3 l = normalize (lightPos-pos);
  vec3 d = lightDir;
  float cosGamma = cos(spotAngle*180.0/PI);
  float spotFactor = 1.0;

  if(dot(-l,d)<cosGamma){ spotFactor=0.0; }

  vec3 n = normalize (normal);
  vec3 v = normalize (-pos);
  vec3 r = reflect(-l,n);

  vec3 diffuse = (Id*getDiffuseColorMethod()*max(dot(l,n),0.0));
  vec3 specular = (Is*Ks*pow(max(dot(r,v),0.0),exponent));
  return vec4 (spotFactor*(diffuse+specular), 1.0);
}


subroutine ( getColor )
vec4 wireframe ()
{ return vec4 ( 0, 0, 0, 1 );
}

void main ()
{ fragColor = getColorMethod();
}

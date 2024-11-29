// FRAGMENT SHADER
#version 410

//Datos VS
in vec3 pos;
in vec3 normal;

//Reflexión Material
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

//Valores iluminación
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform float exponent;
uniform vec lightPos;
 //IMPORTANTE: PASAR LA DIRECCIÓN NORMALIZADA EN COORDENADAS DE VISIÓN
 //VA DESDE EL ORIGEN HACIA LA ESCENA
uniform vec lightDir;
uniform float spotAngle;

out vec4 fragColor;

//Una subrutina para multiplicar cada tipo de luz
//una implementación extra para el modo wireframe
subroutine vec4 getFinalColor();
subroutine uniform getFinalColor getColorMethod;

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
  vec3 r = reflect(-l,n);

  vec3 diffuse = (Id*Kd*max(dot(l,n),0.0));
  vec3 specular = (Is*Ks*pow(max(dot(r,v),0.0),exponent));
  return vec4 (diffuse+specular, 1.0);
}

subroutine ( getColor )
vec4 directionLight ()
{ 
  vec3 l = -lightDir;
  vec3 v = normalize (-pos);
  vec3 r = reflect(-l,n);

  vec3 diffuse = (Id*Kd*max(dot(l,n),0.0));
  vec3 specular = (Is*Ks*pow(max(dot(r,v),0.0),exponent));
  return vec4 ( diffuse+specular, 1.0 );
}

subroutine ( getColor )
vec4 spotLight ()
{ 
  vec3 l = normalize (lightPos-pos);
  vec3 d = lightDirection;
  float cosGamma = cos(spotAngle*180.0/M_PI);
  float spotFactor = 1.0;

  if(dot(-l,d)<cosGamma){
  spotFactor=0.0;
  }
  
  vec3 n = normalize (normal);
  vec3 v = normalize (-pos);
  vec3 r = reflect(-l,n)

  vec3 diffuse = (Id*Kd*max(dot(l,n),0.0));
  vec3 specular = (Is*Ks*pow(max(dot(r,v),0.0),exponent));
  return vec4 (spotFactor*(diffuse+specular), 1.0);
}

subroutine ( getColor )
vec4 wierframe ()
{ return vec4 ( 0, 0, 0, 1 );
}

void main ()
{ fragColor = getColorMethod();
}

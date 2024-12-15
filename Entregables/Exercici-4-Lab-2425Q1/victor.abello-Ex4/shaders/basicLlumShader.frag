#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform vec3 posFocusBlanc;
uniform vec3 posFocusDisco0;
uniform vec3 posFocusDisco1;
uniform vec3 posFocusDisco2;
uniform vec3 posFocusDisco3;


uniform vec3 colorFocusBlanc;
uniform vec3 colorFocusDisco0;
uniform vec3 colorFocusDisco1;
uniform vec3 colorFocusDisco2;
uniform vec3 colorFocusDisco3;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    vec3 fnormSCO = normalize(fnormal);

    vec3 posFocusBlancSCA = (View * vec4(posFocusBlanc, 1)).xyz;
    vec3 posFocusDisco0SCA = (View * vec4(posFocusDisco0, 1)).xyz;
    vec3 posFocusDisco1SCA = (View * vec4(posFocusDisco1, 1)).xyz;
    vec3 posFocusDisco2SCA = (View * vec4(posFocusDisco2, 1)).xyz;
    vec3 posFocusDisco3SCA = (View * vec4(posFocusDisco3, 1)).xyz;

    vec3 Lblanc = normalize(posFocusBlancSCA - fvertex);
    vec3 Ldisco0 = normalize(posFocusDisco0SCA - fvertex);
    vec3 Ldisco1 = normalize(posFocusDisco1SCA - fvertex);
    vec3 Ldisco2 = normalize(posFocusDisco2SCA - fvertex);
    vec3 Ldisco3 = normalize(posFocusDisco3SCA - fvertex);
    
    vec3 color = Ambient() + Difus(fnormSCO, Lblanc, colorFocusBlanc) + Especular(fnormSCO, Lblanc, fvertex, colorFocusBlanc);
    color += Difus(fnormSCO, Ldisco0, colorFocusDisco0) + Especular(fnormSCO, Ldisco0, fvertex, colorFocusDisco0);
    color += Difus(fnormSCO, Ldisco1, colorFocusDisco1) + Especular(fnormSCO, Ldisco1, fvertex, colorFocusDisco1);
    color += Difus(fnormSCO, Ldisco2, colorFocusDisco2) + Especular(fnormSCO, Ldisco2, fvertex, colorFocusDisco2);
    color += Difus(fnormSCO, Ldisco3, colorFocusDisco3) + Especular(fnormSCO, Ldisco3, fvertex, colorFocusDisco3);

    FragColor = vec4(color, 1);
}

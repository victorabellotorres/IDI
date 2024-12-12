#version 330 core

out vec4 FragColor;

in vec3 fvertSCO;
in vec3 fnormSCO;

in vec3 fmatamb;
in vec3 fmatdiff;
in vec3 fmatspec;
in float fmatshin;

uniform vec3 colorFocusSCO;
uniform vec3 colorFocusSCA;
uniform vec3 posFocusSCO;
uniform vec3 posFocusSCA;
uniform mat4 view;
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

vec3 Ambient() {
    return llumAmbient * fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Tant sols retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * fmatdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Tant sols retorna el terme especular!
    // Els vectors rebuts com a paràmetres (NormSCO i L) estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    // Calculem i retornem la component especular
    float shine = pow(max(0.0, dot(R, V)), fmatshin);
    return (fmatspec * colFocus * shine); 
}

void main()
{	

	vec3 fnormSCOnormalized = normalize(fnormSCO);

  // ------------- calcul L --------------
  vec3 LSCO = posFocusSCO - fvertSCO;
  vec3 LSCA = posFocusSCA - fvertSCO;

    LSCO = normalize(LSCO);
    LSCA = normalize(LSCA);

	vec3 color = Ambient() + Difus(fnormSCO, LSCO, colorFocusSCO) + Especular(fnormSCO, LSCO, vec4(fvertSCO, 1.0), colorFocusSCO)+ Difus(fnormSCO,LSCA, colorFocusSCA) + Especular(fnormSCO,LSCA, vec4(fvertSCO, 1.0), colorFocusSCA);
	FragColor = vec4(color, 1.0);
}

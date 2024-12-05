#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 colorFocus;
uniform vec3 posFocus;

out vec3 fvertSCO;
out vec3 fnormSCO;

out vec3 fmatamb;
out vec3 fmatdiff;
out vec3 fmatspec;
out float fmatshin;

void main()
{	

  	// -------------- calcul normSCO ---------------
    mat3 normalMatrix = inverse(transpose(mat3(view*TG)));
    vec3 normSCO = normalMatrix*normal;
    normSCO = normalize(normSCO);

    // ------------- calcul vertSCO ---------------
    vec4 vertSCO = view*TG*vec4(vertex, 1.0);

    gl_Position = proj * vertSCO;

    fvertSCO = vertSCO.xyz;
    fnormSCO = normSCO;
    fmatamb = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;
}

#version 330 core

in vec3 vertex;

uniform float scl;
uniform mat4 TG;

void main()  {
    gl_Position = TG*vec4 (vertex*scl, 1.0);
}

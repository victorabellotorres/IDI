#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 TGpos;

out vec3 fcolor;

void main()  {
    gl_Position = TGpos * TG * vec4 (vertex, 1.0);
    fcolor = color;
}

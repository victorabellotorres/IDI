#version 330 core

in vec3 vertex;

out float pepe;

void main()  {
    gl_Position = vec4 (vertex*0.5, 1.0);
}

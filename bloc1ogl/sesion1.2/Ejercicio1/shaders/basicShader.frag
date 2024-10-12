#version 330 core

 out vec4 FragColor;

 uniform int width;
 uniform int height;

 void main() {

        if (gl_FragCoord.x < width/2.) // la solucion puede ser simplemente poner el width y height hardcodeado
        if (gl_FragCoord.y > height/2.)
            FragColor = vec4(1.,0.,0.,1);
        else 
            FragColor = vec4(1.,1.,0.,1);
    else {
        if (gl_FragCoord.y > height/2.)
            FragColor = vec4(0.,0.,1.,1);
        else 
            FragColor = vec4(0.,1.,0.,1); 
    }

 }



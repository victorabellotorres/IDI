#version 330 core

 out vec4 FragColor;

 void main() {

    FragColor = vec4(1.);
    if (gl_FragCoord.x < 355.) // para no tenerlo hardcodeado se tendrian que llamar a la funcion resize en widget.cpp y luego pasar como uniform el valor y usarlo aqui, tambien hacerlo sin if's es mas eficiente
        if (gl_FragCoord.y > 357.)
            FragColor = vec4(1.,0.,0.,1);
        else 
            FragColor = vec4(1.,1.,0.,1);
    else {
        if (gl_FragCoord.y > 357.)
            FragColor = vec4(0.,0.,1.,1);
        else 
            FragColor = vec4(0.,1.,0.,1); 
    }
 }



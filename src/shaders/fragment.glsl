#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

void main() {
    // gl_PointCoord va de (0,0) abajo-izq a (1,1) arriba-der dentro del punto
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);

    // Descarta fragmentos fuera del radio 0.5 para hacer círculo
    if (dist > 0.5)
        discard;

    FragColor = vertexColor;
}

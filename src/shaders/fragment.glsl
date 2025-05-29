#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);

    if (dist > 0.5) {
        discard;
    }

    // Degradado: mezcla el color base con un color más claro (blanco)
    float t = smoothstep(0.5, 0.0, dist); // 0 en borde, 1 en centro
    vec3 lighter = vec3(1.0, 1.0, 1.0);  // blanco
    vec3 color = mix(vertexColor.rgb, lighter, t * 0.5);  // mezcla parcial para aclarar

    FragColor = vec4(color, vertexColor.a);
}


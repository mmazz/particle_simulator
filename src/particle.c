#include "../include/particle.h"
const float min_bounce_speed = 0.1f;
const int VEL_EXPLOTE = 8;

void add_particles(Particle** particles, int* count, int n_to_add) {
    int new_count = *count + n_to_add;
    Particle* new_array = realloc(*particles, new_count * sizeof(Particle));
    if (!new_array) {
        fprintf(stderr, "Error al realocar memoria para partículas\n");
        return;
    }
    *particles = new_array;

    // Inicializar las nuevas partículas (por ejemplo, en el centro o donde quieras)
    for (int i = *count; i < new_count; i++) {
        particles[0][i].x = 0.0f; // Podés cambiar a la posición deseada
        particles[0][i].y = 0.0f;
        particles[0][i].vx = 0.0f;
        particles[0][i].vy = 0.0f;
        particles[0][i].r = 0.0f;
        particles[0][i].g = 0.0f;
        particles[0][i].b = 1.0f;
        particles[0][i].a = 1.0f;
        particles[0][i].size = 20.0f;
        particles[0][i].life = 10.0f;
    }

    *count = new_count;
}


void update_particles(Particle* particles, int count, float dt) {
    const float floor_y = -1.0f;
    const float bounce = 0.7f;
    const float friction = 0.9f;
    const float gravity = -9.8f;
    const float min_bounce_speed = 0.1f;
    const float v_max = 5.0f;  // velocidad máxima para normalizar color

    for (int i = 0; i < count; i++) {
        // Actualizar velocidad con gravedad
        particles[i].vy += gravity * dt;

        // Actualizar posición
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;

        // Rebote en piso
        if (particles[i].y < floor_y) {
            particles[i].y = floor_y;

            if (fabsf(particles[i].vy) < min_bounce_speed) {
                particles[i].vy = 0.0f;
            } else {
                particles[i].vy = -particles[i].vy * bounce;
            }

            // Rozamiento solo cuando toca piso
            particles[i].vx *= friction;
            if (fabsf(particles[i].vx) < 0.01f) {
                particles[i].vx = 0.0f;
            }
        }

        // Rebote en paredes X
        if (particles[i].x < -1.0f) {
            particles[i].x = -1.0f;
            particles[i].vx = -particles[i].vx * bounce;
        }
        if (particles[i].x > 1.0f) {
            particles[i].x = 1.0f;
            particles[i].vx = -particles[i].vx * bounce;
        }

        // Calcular velocidad y normalizar
        float speed = sqrtf(particles[i].vx * particles[i].vx + particles[i].vy * particles[i].vy);
        float v_norm = speed / v_max;
        if (v_norm > 1.0f) v_norm = 1.0f;

        // Interpolar color: azul (0,0,1) a rojo (1,0,0)
        particles[i].r = v_norm;
        particles[i].g = 0.0f;
        particles[i].b = 1.0f - v_norm;
        particles[i].a = 1.0f;
    }
}

void influence_particles(float x_click, float y_click, Particle* particles, int count) {
    printf("Ejecutando influencia sobre %d partículas\n", count);
    const float radius = 0.2f; // radio de influencia (ajustable)

    for (int i = 0; i < count; i++) {
        float dx = particles[i].x - x_click;
        float dy = particles[i].y - y_click;
        float dist = sqrtf(dx*dx + dy*dy);

        if (dist < radius) {
            // Asignar velocidad aleatoria pequeña
            particles[i].vx = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; // entre -1 y 1
            particles[i].vy = VEL_EXPLOTE*((float)rand() / RAND_MAX);
        }
    }
}


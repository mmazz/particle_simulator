#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
#include <GL/glew.h>    
#include <stdio.h>
#include "../include/shader.h"
#include "../include/particle.h"
#include "../include/renderer.h"

// Settings
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define NUM_PARTICLES 1000

Particle* g_particles = NULL;
int g_num_particles = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Tu código actual para influir partículas...
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float x_ndc = (float)(xpos / width) * 2.0f - 1.0f;
        float y_ndc = 1.0f - (float)(ypos / height) * 2.0f;

        if (g_particles) {
            influence_particles(x_ndc, y_ndc, g_particles, g_num_particles);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        // Añadir más partículas en la posición del click
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float x_ndc = (float)(xpos / width) * 2.0f - 1.0f;
        float y_ndc = 1.0f - (float)(ypos / height) * 2.0f;

        const int n_new = 10;  // cuántas partículas agregar

        add_particles(&g_particles, &g_num_particles, n_new);

        // Inicializar las nuevas partículas en la posición del click con velocidad aleatoria
        for (int i = g_num_particles - n_new; i < g_num_particles; i++) {
            g_particles[i].x = x_ndc;
            g_particles[i].y = y_ndc;
            g_particles[i].vx = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            g_particles[i].vy = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            g_particles[i].r = 1.0f;  // color rojo, por ejemplo
            g_particles[i].g = 0.0f;
            g_particles[i].b = 0.0f;
            g_particles[i].a = 1.0f;
            g_particles[i].size = 20.0f;
            g_particles[i].life = 10.0f;
        }

        // Actualizar el VBO con el nuevo tamaño
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, g_num_particles * sizeof(Particle), NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, g_num_particles * sizeof(Particle), g_particles);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}


int inicialization(Particle* particles){
    if (!particles) {
        fprintf(stderr, "Error al asignar memoria\n");
        return -1;
    }
    for (int i = 0; i < g_num_particles; ++i) {
        particles[i].x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; // entre -1 y 1
        particles[i].y = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        particles[i].vx = 0.0f;
        particles[i].vy = 0.0f;
        particles[i].r = 1.0f;
        particles[i].g = 1.0f;
        particles[i].b = 0.5f;
        particles[i].a = 0.85f; // color
        particles[i].size = 20.0f;
        particles[i].life = 10;
    }
    return 0;
}
int main(void) {

    // Inicializar GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Error: No se pudo inicializar GLFW\n");
        return -1;
    }

    // Crear ventana con contexto OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Verlet Integration", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Error: No se pudo crear la ventana GLFW\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Para usar funciones modernas
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Error: No se pudo inicializar GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    
    int num_particles = NUM_PARTICLES;

    // Luego reservá memoria para ellas
    Particle* particles = malloc(num_particles * sizeof(Particle));
    if (!particles) {
        fprintf(stderr, "Error al asignar memoria para partículas\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    g_particles = particles;
    g_num_particles = num_particles;




    init_render(num_particles);
    inicialization(particles);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint shader_program = load_shader_program("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

    if (!shader_program) {
        fprintf(stderr, "No se pudo cargar el shader program\n");
        return -1;
    }
    // Viewport inicial
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Procesar eventos
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Aquí iría la actualización y renderizado de partículas
        update_particles(g_particles, g_num_particles, deltaTime);
        glUseProgram(shader_program);
        // Renderizar partículas
        render_particles(g_particles, g_num_particles);
        // Swap buffers (mostrar lo que se dibujó)
        glfwSwapBuffers(window);
    }
    cleanup_render();
    // Limpieza
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

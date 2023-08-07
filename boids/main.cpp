#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "boidManager.h"
#include <vector>

#include <iostream> 

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

std::vector<vector_2> boidCenters{
	{0.0f, 0.0f, 0},
	{0.1f, 0.0f, 0},
	{0.1f, 0.1f, 0},
	{0.0f, 0.1f, 0},
	{-0.1f, 0.0f, 0},
	{-0.1f, -0.1f, 0},
	{0.0f, -0.1f, 0}
};


// ... (Vertex and Fragment Shader source code, boidCenters vector declaration) ...

int main()
{
    boidManager boids(boidCenters);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    // ... (GLFW window initialization and OpenGL context setup) ...
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int monitor_width = mode->width; // Monitor's width.
    int monitor_height = mode->height;

    int window_width = (int)(monitor_width * 0.5f); // Window size will be 50% the monitor's size...
    int window_height = (int)(monitor_width * 0.5f); // ... Cast is simply to silence the compiler warning.

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "GLFW Test Window", NULL, NULL);
    // GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Drawing Basic Shapes - Buffer Objects & Shaders", glfwGetPrimaryMonitor(), NULL); // Full Screen Mode ("Alt" + "F4" to Exit!)

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Introduce the window into the current context
    glfwMakeContextCurrent(window);


    gladLoadGL();
    glViewport(0, 0, window_width, window_height);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, boids.getAllVertSize() * sizeof(GLfloat), boids.getAllVert(), GL_STREAM_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, boids.getAllIndicesSize() * sizeof(GLuint), boids.getAllIndices(), GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        boids.updateBoids();

        // Update VBO data if needed
        if (boids.isUpdated())
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, boids.getAllVertSize() * sizeof(GLfloat), boids.getAllVert());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            boids.setUpdated(false);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, boids.getAllIndicesSize(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


/*TODO
1. figure out better rendering system
2. better loop for consistency

3. boid follow AI
*/
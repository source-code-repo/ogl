#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogl.h"
#include "triangle.h"
#include "drawable.h"
#include "shader_loader.h"

using namespace glm;

const char* VERTEX_SHADER = "shaders/vertex.glsl";
const char* FRAGMENT_SHADER = "shaders/fragment.glsl";

int main()
{
    GLFWwindow* window;
    if (initWindow(window)) {
        std::cout << "Error occurred, exiting\n";
        return -1;
    }
    // Create objects to draw
    std::shared_ptr<Triangle> triangle(new Triangle());
    triangle->buffer();
    std::vector<std::shared_ptr<Drawable>> drawables;
    drawables.push_back(triangle);
    GLuint programId = initShaders();
    GLuint mvpMatrix = glGetUniformLocation(programId, "mvp");
    // Main loop
    do {
        // Clear the screen to avoid flickering
        glClear(GL_COLOR_BUFFER_BIT);
        // Use shaders
        glUseProgram(programId);
        glm::mat4 mvp = project();
        // Transform model into world space then camera space then 2d projection
        glUniformMatrix4fv(mvpMatrix, 1, GL_FALSE, &mvp[0][0]);
        // Draw objects
        for (auto const& value : drawables) {
            value->draw();
        }
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}

/*! @brief Creates a window, makes it the current OpenGL context
 *  @param[out] The initialised GLFWwindow
 */
int initWindow(GLFWwindow*& window)
{
    // Initialise GLFW
    if (glfwInit() != GL_TRUE) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
    //GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, "
            "they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Initialize GLEW
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    return 0;
}

GLuint initShaders() {
    ShaderLoader loader;
    std::stringstream vertexCode = loader.getCode(VERTEX_SHADER);
    std::stringstream frgmntCode = loader.getCode(FRAGMENT_SHADER);
    GLuint vertexShaderId = loader.compileShader(vertexCode.str().c_str(), GL_VERTEX_SHADER);
    GLuint frgmntShaderId = loader.compileShader(frgmntCode.str().c_str(), GL_FRAGMENT_SHADER);
    // Link program
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, frgmntShaderId);
    glLinkProgram(programId);
    // Check the program
    checkShaders(programId);
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, frgmntShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(frgmntShaderId);
    return programId;
}

void checkShaders(const GLuint& programId)
{
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> error(infoLogLength + 1);
        glGetProgramInfoLog(programId, infoLogLength, NULL, &error[0]);
        printf("%s\n", &error[0]);
    }
}

/*! @brief Applies model, view and projection using shaders
 *  @param Shader program ID
 */
glm::mat4 project() {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)4.0f / (float)3.0f, 0.1f, 100.0f);

    // Or, for an ortho camera :
    glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    glm::mat4 view = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = projection * view * model; // Remember, matrix multiplication is the other way around
    return mvp;
}

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
    mainLoop(window, drawables, programId);
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
    checkProgram(programId);
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, frgmntShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(frgmntShaderId);
    return programId;
}

void checkProgram(const GLuint& programId)
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

/*! @brief Main loop, draws the buffer to the window
 */
void mainLoop(GLFWwindow* window, std::vector<std::shared_ptr<Drawable>> drawables, GLuint shaderProgramId)
{
    do {
        // Clear the screen to avoid flickering
        glClear(GL_COLOR_BUFFER_BIT);
        // Use shaders
        glUseProgram(shaderProgramId);
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
}
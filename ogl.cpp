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

using namespace glm;

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

    mainLoop(window, drawables);
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

/*! @brief Main loop, draws the buffer to the window
 */
void mainLoop(GLFWwindow* window, std::vector<std::shared_ptr<Drawable>> drawables)
{
    do {
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT);

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
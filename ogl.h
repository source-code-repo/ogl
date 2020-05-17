#pragma once
#include <vector>
#include "drawable.h"
#include "triangle.h"

void mainLoop(GLFWwindow* window, std::vector<std::shared_ptr<Drawable>> drawables, GLuint shaderProgramId);
int main();
int initWindow(GLFWwindow*& window);
GLuint initShaders();

void checkProgram(const GLuint& programId);

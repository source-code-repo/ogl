#pragma once
#include <vector>
#include "drawable.h"
#include "triangle.h"

int main();
int initWindow(GLFWwindow*& window);
GLuint initShaders();
void checkShaders(const GLuint& programId);
glm::mat4 project();
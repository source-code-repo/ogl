// Vertex shader. Reduces size by 50%
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
void main(){
  gl_Position.xyz = vertexPosition_modelspace * 0.5;
  gl_Position.w = 1.0;
}
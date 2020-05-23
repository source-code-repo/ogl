// Vertex shader, applies model, view and projection transforms
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 mvp; // Model, view, projection
void main(){
  // Output position of the vertex, in clip space : mvp * position
  gl_Position =  mvp * vec4(vertexPosition_modelspace,1);
}
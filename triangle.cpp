#include <stdio.h>
#include <GL/glew.h>

#include "triangle.h"

void Triangle::buffer() {
    // Vertex array object
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    // Vetices
    const GLfloat g_vertex_buffer_data[] = {
        -0.9f, -0.9f, 0.0f,
        0.9f, -0.9f, 0.0f,
        0.0f,  0.9f, 0.0f,
    };
    // This will identify our vertex buffer
    GLuint vb;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vb);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    this->vertexbuffer = vb;
}

void Triangle::draw() {
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
}
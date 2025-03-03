#include "Pyramid.h"
#include "ShaderManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include"camera.h"
extern ShaderManager* globalShader;
extern Camera camera;

// Pyramid vertices and indices
std::vector<float> pyramidVertices = {
    // Positions               // Normals              // Texture Coords
    // Top vertex
    0.0f,  1.0f,  0.0f,   0.0f,  1.0f,  0.0f,   0.5f, 1.0f, // Top vertex

    // Base of the pyramid (4 vertices)
    -1.0f, -1.0f,  1.0f,   -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, // Front-left
     1.0f, -1.0f,  1.0f,   0.5f, -0.5f,  0.5f,   1.0f, 0.0f, // Front-right
     1.0f, -1.0f, -1.0f,   0.5f, -0.5f,  -0.5f,   1.0f, 1.0f, // Back-right
    -1.0f, -1.0f, -1.0f,   -0.5f, -0.5f, -0.5f,   0.0f, 1.0f  // Back-left
};

// Indices for the pyramid faces
std::vector<unsigned int> pyramidIndices = {
    0, 1, 2, // Front face
    0, 2, 3, // Right face
    0, 3, 4, // Back face
    0, 4, 1, // Left face
    1, 2, 3, // Base triangle 1
    1, 3, 4  // Base triangle 2
};

// This function computes the cross product of two vectors to get the normal
glm::vec3 computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    glm::vec3 v1 = p2 - p1;
    glm::vec3 v2 = p3 - p1;
    return glm::normalize(glm::cross(v1, v2));
}

void Pyramid::Render(const glm::mat4& projection, const glm::mat4& view) {

    // Pyramid buffers
    unsigned int pyramidVAO, pyramidVBO, pyramidEBO;
    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);
    glGenBuffers(1, &pyramidEBO);

    glBindVertexArray(pyramidVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    glBufferData(GL_ARRAY_BUFFER, pyramidVertices.size() * sizeof(float), &pyramidVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pyramidIndices.size() * sizeof(unsigned int), &pyramidIndices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (3 floats: nx, ny, nz)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates attribute (2 floats: s, t)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    globalShader->Use();

    // Set light properties for Phong shading
    // Light properties (You can customize the values or update them dynamically)
    //glm::vec3 lightPos(2.0f, 5.5f, 6.0f);  // Example light position
    //glm::vec3 viewPos(0.0f, 0.0f, 8.0f);   // Camera position (view position)
   // globalShader->SetUniform3f("lightPos", lightPos);
    //globalShader->SetUniform3f("viewPos", viewPos);

    glm::vec3 ambientColor(0.5f, 0.1f, 0.1f);  // Soft blue ambient color
    glm::vec3 diffuseColor(0.8f, 0.3f, 0.3f);  // Light blue for diffuse
    glm::vec3 specularColor(1.0f, 0.5f, 0.5f); // Light blue specular highlights
    float shininess = 32.0f;  // Shininess coefficient

    // Set material properties
    globalShader->SetUniform3f("material.ambient", ambientColor);
    globalShader->SetUniform3f("material.diffuse", diffuseColor);
    globalShader->SetUniform3f("material.specular", specularColor);
    globalShader->SetUniform1f("material.shininess", shininess);

    // Set up transformation matrices
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    // Rotation - rotate around the Y-axis (you can change the axis to x or z)
    float angle = static_cast<float>(glfwGetTime() * -50.0f); // Rotate over time, 50 degrees per second
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
    globalShader->SetUniformMatrix4f("model", model);
    globalShader->SetUniformMatrix4f("view", view);
    globalShader->SetUniformMatrix4f("projection", projection);

    // OpenGL rendering code for the pyramid (VAO, VBO setup, etc.)
    glBindVertexArray(pyramidVAO);
    glDrawElements(GL_TRIANGLES, pyramidIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


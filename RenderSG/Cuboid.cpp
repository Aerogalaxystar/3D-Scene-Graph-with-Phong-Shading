#include "Cuboid.h"
#include "ShaderManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.h"
extern ShaderManager* globalShader;
extern Camera camera;
std::vector<float> cuboidVertices = {
    // Positions               // Normals             // Texture Coords
    // Front face
    -2.0f, -0.5f, -2.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Front-left-bottom
     2.0f, -0.5f, -2.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // Front-right-bottom
     2.0f,  1.0f, -2.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // Front-right-top
    -2.0f,  1.0f, -2.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // Front-left-top

    // Back face
    -2.0f, -0.5f,  2.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // Back-left-bottom
     2.0f, -0.5f,  2.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // Back-right-bottom
     2.0f,  1.0f,  2.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // Back-right-top
    -2.0f,  1.0f,  2.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // Back-left-top

    // Left face
    -2.0f, -0.5f, -2.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // Front-left-bottom
    -2.0f, -0.5f,  2.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // Back-left-bottom
    -2.0f,  1.0f,  2.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // Back-left-top
    -2.0f,  1.0f, -2.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // Front-left-top

    // Right face
     2.0f, -0.5f, -2.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // Front-right-bottom
     2.0f, -0.5f,  2.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // Back-right-bottom
     2.0f,  1.0f,  2.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // Back-right-top
     2.0f,  1.0f, -2.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // Front-right-top

     // Bottom face
     -2.0f, -0.5f, -2.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // Front-left-bottom
      2.0f, -0.5f, -2.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // Front-right-bottom
      2.0f, -0.5f,  2.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // Back-right-bottom
     -2.0f, -0.5f,  2.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // Back-left-bottom

     // Top face
     -2.0f,  1.0f, -2.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, // Front-left-top
      2.0f,  1.0f, -2.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // Front-right-top
      2.0f,  1.0f,  2.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // Back-right-top
     -2.0f,  1.0f,  2.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f  // Back-left-top
};


std::vector<unsigned int> cuboidIndices = {
    // Front face (2 triangles)
    0, 1, 2,  0, 2, 3,
    // Back face (2 triangles)
    4, 5, 6,  4, 6, 7,
    // Left face (2 triangles)
    8, 9, 10,  8, 10, 11,
    // Right face (2 triangles)
    12, 13, 14,  12, 14, 15,
    // Bottom face (2 triangles)
    16, 17, 18,  16, 18, 19,
    // Top face (2 triangles)
    20, 21, 22,  20, 22, 23
};
void Cuboid::Render(const glm::mat4& projection, const glm::mat4& view) {

    //Cuboid Buffers
    unsigned int cuboidVAO, cuboidVBO, cuboidEBO;
    glGenVertexArrays(1, &cuboidVAO);
    glGenBuffers(1, &cuboidVBO);
    glGenBuffers(1, &cuboidEBO);

    glBindVertexArray(cuboidVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cuboidVBO);
    glBufferData(GL_ARRAY_BUFFER, cuboidVertices.size() * sizeof(float), &cuboidVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cuboidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cuboidIndices.size() * sizeof(unsigned int), &cuboidIndices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates attribute (if you're using them for textures)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    globalShader->Use();
    globalShader->SetUniform1i("material.diffuse", 0);

    // Light properties (You can customize the values or update them dynamically)
    //glm::vec3 lightPos(1.0f, -0.5f, 4.0f);  // Light position
    //glm::vec3 lightColor(1.0f, 1.0f, 1.0f);  // White light

   /* globalShader->SetUniform3f("lightPos", lightPos);
    globalShader->SetUniform3f("lightColor", lightColor);

    // Material properties (can be customized per object)
    // Material properties (can be customized per object)
    glm::vec3 ambientColor(0.1f, 0.3f, 0.1f); // Soft light green ambient light
    glm::vec3 diffuseColor(0.4f, 0.8f, 0.4f); // Light green diffuse
    glm::vec3 specularColor(0.6f, 1.0f, 0.6f); // Lighter green specular reflection (shiny light green)

    globalShader->SetUniform3f("ambientColor", ambientColor);
    globalShader->SetUniform3f("diffuseColor", diffuseColor);
    globalShader->SetUniform3f("specularColor", specularColor);

    globalShader->SetUniform1f("shininess", 32.0f);  // Example shininess

    float shininess = 32.0f; // Shininess factor for specular reflection

    globalShader->SetUniform3f("ambientColor", ambientColor);
    globalShader->SetUniform3f("diffuseColor", diffuseColor);
    globalShader->SetUniform3f("specularColor", specularColor);
    globalShader->SetUniform1f("shininess", shininess);*/

    // Set material properties
    glm::vec3 ambientColor(0.05f, 0.1f, 0.8f);  // Much more bluish ambient color
    glm::vec3 diffuseColor(0.1f, 0.2f, 1.0f);   // Strong blue for diffuse
    glm::vec3 specularColor(0.2f, 0.4f, 1.0f);  // Bluish specular highlights with strong blue


    float shininess = 32.0f;  // Shininess coefficient

    // Set material properties
    globalShader->SetUniform3f("material.ambient", ambientColor);
    globalShader->SetUniform3f("material.diffuse", diffuseColor);
    globalShader->SetUniform3f("material.specular", specularColor);
    globalShader->SetUniform1f("material.shininess", shininess);
    // Set up transformation matrices
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, size);  // Apply scaling for cuboid

    // Rotation - rotate around the Y-axis (you can change the axis to x or z)
    float angle = static_cast<float>(glfwGetTime() * 50.0f); // Rotate over time, 50 degrees per second
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis

    globalShader->SetUniformMatrix4f("model", model);
    globalShader->SetUniformMatrix4f("view", view);
    globalShader->SetUniformMatrix4f("projection", projection);
    // Set up transformation matrices    

    //globalShader->SetUniform3f("objectColor", color);
    //glm::vec4 emissiveCuboidColor = glm::vec4(0.2f, 0.4f, 1.0f, 1.0f); // Bluish glow
    //globalShader->SetUniform4f("emissiveColor", emissiveCuboidColor);

    // OpenGL rendering code for the cuboid (VAO, VBO setup, etc.)
    glBindVertexArray(cuboidVAO);
    glDrawElements(GL_TRIANGLES, cuboidIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

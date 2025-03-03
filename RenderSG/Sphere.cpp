#include "Sphere.h"
#include "ShaderManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.h"
extern ShaderManager* globalShader;
extern ShaderManager* sphereShader;
extern Camera camera;
// Sphere generation
void createSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, int sectorCount, int stackCount) {
    const float PI = 3.14159265359f;

    // Step 1: Generate vertices with positions, normals, and texture coordinates
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2 - i * PI / stackCount; // From pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);           // r * cos(u)
        float z = radius * sinf(stackAngle);            // r * sin(u)

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * PI / sectorCount; // 0 to 2pi

            // Vertex positions
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float zPos = z;

            // Normal (normalized position vector for sphere)
            float nx = x / radius;
            float ny = y / radius;
            float nz = zPos / radius;

            // Texture coordinates
            float s = (float)j / sectorCount;
            float t = (float)i / stackCount;

            // Add position, normal, and texture coordinates to the vertices array
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(zPos);

            vertices.push_back(nx); // Normal x
            vertices.push_back(ny); // Normal y
            vertices.push_back(nz); // Normal z

            vertices.push_back(s);  // Texture coord s
            vertices.push_back(t);  // Texture coord t
        }
    }

    // Step 2: Generate indices for the sphere
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1); // Beginning of the current stack
        int k2 = k1 + sectorCount + 1;  // Beginning of the next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // Two triangles per sector
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}


void Sphere::Render(const glm::mat4& projection, const glm::mat4& view) {
    // Sphere data
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    // Define the orbital parameters
        float orbitRadius = 5.0f;  // Orbit radius
        float orbitSpeed = 0.5f;   // Speed of orbit (adjust for faster/slower orbit)

        // Calculate the orbital angle based on time
        float orbitalAngle = static_cast<float>(glfwGetTime() * orbitSpeed); // Time-based angle for orbital motion

        // Calculate the position of the sphere in orbital motion using sin and cos
        glm::vec3 orbitalPosition;
        orbitalPosition.x = orbitRadius * cos(orbitalAngle); // X position
        orbitalPosition.z = orbitRadius * sin(orbitalAngle); // Z position
        orbitalPosition.y = 0.0f; // You can change this to make the sphere move up/down as well

        // Define the orbital parameters
        float orbitRadiusUD = 5.0f;  // Orbit radius (for Z axis)
        float orbitSpeedUD = 0.5f;    // Speed of orbit (adjust for faster/slower orbit)
        float orbitHeightUD = 5.0f;   // Height for up-down motion (adjust for more/less vertical movement)

        // Calculate the orbital angle based on time
        float orbitalAngleUD = static_cast<float>(glfwGetTime() * orbitSpeedUD); // Time-based angle for orbital motion

        // Calculate the position of the sphere in orbital motion using sin and cos
        glm::vec3 orbitalPositionUD;
        orbitalPositionUD.x = 0.0f;  // X position is fixed (0) for a vertical orbital path
        orbitalPositionUD.z = orbitRadiusUD * cos(orbitalAngleUD);  // Z position (circular motion along Z-axis)
        orbitalPositionUD.y = orbitHeightUD * sin(orbitalAngleUD);  // Y position for up-down motion (sinusoidal)

        // Define the orbital parameters
        float orbitRadiusS3 = 6.0f;  // Orbit radius (for X and Z axis)
        float orbitSpeedS3 = 0.9f;    // Speed of orbit (adjust for faster/slower orbit)

        // Calculate the orbital angle based on time
        float orbitalAngleS3 = static_cast<float>(glfwGetTime() * orbitSpeedS3); // Time-based angle for orbital motion

        // Calculate the position of the sphere in orbital motion using sin and cos
        float x1 = orbitRadiusS3 * cos(orbitalAngleS3); // X position (circular motion in XZ plane)
        float z1 = orbitRadiusS3 * sin(orbitalAngleS3); // Z position (circular motion in XZ plane)
        float y1 = 3.5f * sin(orbitalAngleS3); // Y position (up-down motion based on sinusoidal function)

        // Create the position vector
        glm::vec3 positionS3 = glm::vec3(x1, y1, z1);

        // Define the orbital parameters
        float orbitRadiusS4 = 6.0f;  // Orbit radius (for X and Z axis)
        float orbitSpeedS4 = 0.7f;    // Speed of orbit (adjust for faster/slower orbit)

        // Calculate the orbital angle based on time
        float orbitalAngleS4 = static_cast<float>(glfwGetTime() * orbitSpeedS4); // Time-based angle for orbital motion

        // Calculate the position of the sphere in orbital motion using sin and cos
        float x2 = orbitRadiusS4 * cos(-orbitalAngleS4); // X position (circular motion in XZ plane)
        float z2 = orbitRadiusS4 * sin(-orbitalAngleS4); // Z position (circular motion in XZ plane)
        float y2 = -3.5f * sin(-orbitalAngleS4); // Y position (up-down motion based on sinusoidal function)

        // Create the position vector
        glm::vec3 positionS4 = glm::vec3(x2, y2, z2);


    if (value == 1)
    {
        createSphere(vertices, indices, radius, 36, 18);
        unsigned int sVAO, sVBO, sEBO;
        glGenVertexArrays(1, &sVAO);
        glGenBuffers(1, &sVBO);
        glGenBuffers(1, &sEBO);

        glBindVertexArray(sVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute (3 floats: nx, ny, nz)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glBindVertexArray(0);

        sphereShader->Use();
        

        // Set up transformation matrices
        

        // Set up the model matrix for orbital motion
        glm::mat4 model = glm::translate(glm::mat4(1.0f), orbitalPosition); // Translate the sphere to the orbital position
       // glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        // Now apply rotation around the Y-axis (or any other axis you want)
        float rotationAngle = static_cast<float>(glfwGetTime() * -50.0f); // Rotate over time, 50 degrees per second
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis

        // Set the color (e.g., yellow)
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

        sphereShader->SetUniformMatrix4f("model", model);
        sphereShader->SetUniformMatrix4f("view", view);
        sphereShader->SetUniformMatrix4f("projection", projection);
        sphereShader->SetUniform3f("objectColor", color);
        // OpenGL rendering code for the sphere (VAO, VBO setup, etc.)
         // Draw the sphere
        glBindVertexArray(sVAO); // Bind the VAO for the current draw call
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Or glDrawArrays if no index buffer
        glBindVertexArray(0); // Unbind the VAO after drawing
        return;
    }

    if (value == 2)
    {
        createSphere(vertices, indices, radius, 36, 18);
        unsigned int sVAO, sVBO, sEBO;
        glGenVertexArrays(1, &sVAO);
        glGenBuffers(1, &sVBO);
        glGenBuffers(1, &sEBO);

        glBindVertexArray(sVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute (3 floats: nx, ny, nz)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glBindVertexArray(0);

        sphereShader->Use();


        // Set up transformation matrices


        // Set up the model matrix for orbital motion
        glm::mat4 model = glm::translate(glm::mat4(1.0f), orbitalPositionUD); // Translate the sphere to the orbital position
        // glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
         // Now apply rotation around the Y-axis (or any other axis you want)
        float rotationAngle = static_cast<float>(glfwGetTime() * -50.0f); // Rotate over time, 50 degrees per second
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis

        // Set the color (e.g., yellow)
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

        sphereShader->SetUniformMatrix4f("model", model);
        sphereShader->SetUniformMatrix4f("view", view);
        sphereShader->SetUniformMatrix4f("projection", projection);
        sphereShader->SetUniform3f("objectColor", color);
        // OpenGL rendering code for the sphere (VAO, VBO setup, etc.)
         // Draw the sphere
        glBindVertexArray(sVAO); // Bind the VAO for the current draw call
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Or glDrawArrays if no index buffer
        glBindVertexArray(0); // Unbind the VAO after drawing
        return;
    }
    if (value == 3)
    {
        createSphere(vertices, indices, radius, 36, 18);
        unsigned int sVAO, sVBO, sEBO;
        glGenVertexArrays(1, &sVAO);
        glGenBuffers(1, &sVBO);
        glGenBuffers(1, &sEBO);

        glBindVertexArray(sVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute (3 floats: nx, ny, nz)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glBindVertexArray(0);

        sphereShader->Use();


        // Set up transformation matrices


        // Set up the model matrix for orbital motion
        glm::mat4 model = glm::translate(glm::mat4(1.0f), positionS3); // Translate the sphere to the orbital position
        // glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
         // Now apply rotation around the Y-axis (or any other axis you want)
        float rotationAngle = static_cast<float>(glfwGetTime() * -50.0f); // Rotate over time, 50 degrees per second
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis

        // Set the color (e.g., yellow)
        glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);

        sphereShader->SetUniformMatrix4f("model", model);
        sphereShader->SetUniformMatrix4f("view", view);
        sphereShader->SetUniformMatrix4f("projection", projection);
        sphereShader->SetUniform3f("objectColor", color);
        // OpenGL rendering code for the sphere (VAO, VBO setup, etc.)
         // Draw the sphere
        glBindVertexArray(sVAO); // Bind the VAO for the current draw call
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Or glDrawArrays if no index buffer
        glBindVertexArray(0); // Unbind the VAO after drawing
        return;
    }
    if (value == 4)
    {
        createSphere(vertices, indices, radius, 36, 18);
        unsigned int sVAO, sVBO, sEBO;
        glGenVertexArrays(1, &sVAO);
        glGenBuffers(1, &sVBO);
        glGenBuffers(1, &sEBO);

        glBindVertexArray(sVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute (3 floats: nx, ny, nz)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glBindVertexArray(0);

        sphereShader->Use();


        // Set up transformation matrices


        // Set up the model matrix for orbital motion
        glm::mat4 model = glm::translate(glm::mat4(1.0f), positionS4); // Translate the sphere to the orbital position
        // glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
         // Now apply rotation around the Y-axis (or any other axis you want)
        float rotationAngle = static_cast<float>(glfwGetTime() * -50.0f); // Rotate over time, 50 degrees per second
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis

        // Set the color (e.g., yellow)
        glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);

        sphereShader->SetUniformMatrix4f("model", model);
        sphereShader->SetUniformMatrix4f("view", view);
        sphereShader->SetUniformMatrix4f("projection", projection);
        sphereShader->SetUniform3f("objectColor", color);
        // OpenGL rendering code for the sphere (VAO, VBO setup, etc.)
         // Draw the sphere
        glBindVertexArray(sVAO); // Bind the VAO for the current draw call
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Or glDrawArrays if no index buffer
        glBindVertexArray(0); // Unbind the VAO after drawing
        return;
    }
    createSphere(vertices, indices, radius, 36, 18);

   
    // Vertex buffer and attribute setup
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (3 floats: nx, ny, nz)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates attribute (2 floats: s, t)
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    globalShader->Use();
    globalShader->SetUniform1i("material.diffuse", 0);

    // Set light positions (these are the positions from your orbital calculations)
    globalShader->SetUniform3f("lights[0].position", orbitalPosition);  // Light 1 position (Yellow)
    globalShader->SetUniform3f("lights[1].position", orbitalPositionUD); // Light 2 position (Red)
    globalShader->SetUniform3f("lights[2].position", positionS3);       // Light 3 position (Green)
    globalShader->SetUniform3f("lights[3].position", positionS4);       // Light 4 position (Blue)

    // Set the light properties for each light
    glm::vec3 lightAmbient1(0.2f, 0.2f, 0.0f);  // Yellow (light 1)
    glm::vec3 lightDiffuse1(0.5f, 0.5f, 0.0f);  // Yellow
    glm::vec3 lightSpecular1(1.0f, 1.0f, 0.0f); // Yellow

    glm::vec3 lightAmbient2(0.2f, 0.0f, 0.0f);  // Red (light 2)
    glm::vec3 lightDiffuse2(0.5f, 0.0f, 0.0f);  // Red
    glm::vec3 lightSpecular2(1.0f, 0.0f, 0.0f); // Red

    glm::vec3 lightAmbient3(0.0f, 0.2f, 0.0f);  // Green (light 3)
    glm::vec3 lightDiffuse3(0.0f, 0.5f, 0.0f);  // Green
    glm::vec3 lightSpecular3(0.0f, 1.0f, 0.0f); // Green

    glm::vec3 lightAmbient4(0.0f, 0.0f, 0.2f);  // Blue (light 4)
    glm::vec3 lightDiffuse4(0.0f, 0.0f, 0.5f);  // Blue
    glm::vec3 lightSpecular4(0.0f, 0.0f, 1.0f); // Blue

    // Set uniform values for all lights (ambient, diffuse, specular)
    globalShader->SetUniform3f("lights[0].ambient", lightAmbient1);
    globalShader->SetUniform3f("lights[0].diffuse", lightDiffuse1);
    globalShader->SetUniform3f("lights[0].specular", lightSpecular1);

    globalShader->SetUniform3f("lights[1].ambient", lightAmbient2);
    globalShader->SetUniform3f("lights[1].diffuse", lightDiffuse2);
    globalShader->SetUniform3f("lights[1].specular", lightSpecular2);

    globalShader->SetUniform3f("lights[2].ambient", lightAmbient3);
    globalShader->SetUniform3f("lights[2].diffuse", lightDiffuse3);
    globalShader->SetUniform3f("lights[2].specular", lightSpecular3);

    globalShader->SetUniform3f("lights[3].ambient", lightAmbient4);
    globalShader->SetUniform3f("lights[3].diffuse", lightDiffuse4);
    globalShader->SetUniform3f("lights[3].specular", lightSpecular4);

    // Set camera position
    glm::vec3 viewPos(0.0f, 0.0f, 3.0f);   // Camera position (view position)
    globalShader->SetUniform3f("viewPos", viewPos); // Set camera position

    // Define material properties
    glm::vec3 ambientColor(0.2f, 0.3f, 0.6f);  // Soft blue ambient color
    glm::vec3 diffuseColor(0.4f, 0.6f, 1.0f);  // Light blue for diffuse
    glm::vec3 specularColor(0.6f, 0.8f, 1.0f); // Light blue specular highlights
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

    // OpenGL rendering code for the sphere (VAO, VBO setup, etc.)
    // Draw the sphere
    glBindVertexArray(VAO); // Bind the VAO for the current draw call
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Or glDrawArrays if no index buffer
    glBindVertexArray(0); // Unbind the VAO after drawing

}
 
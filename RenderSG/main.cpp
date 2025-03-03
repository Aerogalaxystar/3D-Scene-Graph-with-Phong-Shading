
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderManager.h"
#include "SceneGraph.h"
#include "Sphere.h"
#include "Pyramid.h"
#include "Cuboid.h"
#include "Camera.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
// Global shader manager
ShaderManager* globalShader = nullptr;
ShaderManager* sphereShader = nullptr;
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

// GLFW mouse and keyboard input variables
bool firstMouse = true;
float lastX = 400, lastY = 300; // Assuming window size is 800x600
bool wireframeMode = false;


// Global deltaTime tracking
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
// Shader program
const char* vertexShaderSource = R"(
 #version 330 core
  layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aNormal;
    layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;  // Fragment position
out vec3 Normal;   // Normal vector
out vec2 TexCoord; // Texture coordinates

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0)); // Transform vertex position to world space
    Normal = mat3(transpose(inverse(model))) * aNormal; // Transform normal to world space
    TexCoord = aTexCoord;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
    )";

const char* fragmentShaderSource = R"(
    #version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 viewPos;
uniform Material material;
uniform Light lights[4];  // Array of lights (4 lights)

void main()
{
    vec3 result = vec3(0.0f);
    
    // Iterate through all lights
    for (int i = 0; i < 4; i++) {
        // Ambient lighting
        vec3 ambient = lights[i].ambient * material.ambient;
        
        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lights[i].diffuse * (diff * material.diffuse);
        
        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lights[i].specular * (spec * material.specular);
        
        result += ambient + diffuse + specular;
    }
    
    FragColor = vec4(result, 1.0);
}
)";


const char* vertexCubeShader =  R"(
  #version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
 )"; 
const char* fragmentCubeShader = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 objectColor; // Color passed from the C++ code

void main()
{
    FragColor = vec4(objectColor,1.0); // Set the color as the output
}
)";
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW Initialization failed!" << std::endl;
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL Scene Graph", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }
    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);  // Enable vsync

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW Initialization failed!" << std::endl;
        return -1;
    }

    // Initialize the global shader with shader source code
    globalShader = new ShaderManager(vertexShaderSource, fragmentShaderSource);
    sphereShader = new ShaderManager(vertexCubeShader, fragmentCubeShader);

    // Create the SceneGraph
    SceneGraph scene;

    // Create objects
    Sphere* sphere = new Sphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    Pyramid* pyramid = new Pyramid(glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Cuboid* cuboid = new Cuboid(glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Sphere* s1sphere = new Sphere(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.7f,1);
    Sphere* s2sphere = new Sphere(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.7f, 2);
    Sphere* s3sphere = new Sphere(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.7f, 3);
    Sphere* s4sphere = new Sphere(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.7f, 4);


    // Set up hierarchy (adding children)
    pyramid->AddChild(sphere);
    sphere->AddChild(cuboid);
    //S1sphere->AddChild(cuboid);   

    // Add root objects to the scene graph
    //scene.AddObject(cuboid);
    scene.AddObject(pyramid);
    scene.AddObject(s1sphere);
    scene.AddObject(s2sphere);
    scene.AddObject(s3sphere);
    scene.AddObject(s4sphere);


    glEnable(GL_DEPTH_TEST);
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        // Set polygon mode based on wireframeMode
        if (wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Render as wireframe
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Render as filled triangles
        }
       

        // Render the scene (Iterate over objects)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        
 
        scene.Render(projection, view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete globalShader;  // Cleanup

    glfwTerminate();
    return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
// Input processing
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool zKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !zKeyPressed) {
        wireframeMode = !wireframeMode;
        zKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) {
        zKeyPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
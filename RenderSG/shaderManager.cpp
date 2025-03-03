#include "ShaderManager.h"

ShaderManager::ShaderManager(const std::string& vertexSource, const std::string& fragmentSource) {
    // Compile the shaders
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // Link shaders into a program
    LinkProgram(vertexShader, fragmentShader);
}

ShaderManager::~ShaderManager() {
    glDeleteProgram(shaderProgram);  // Delete shader program when no longer needed
}

void ShaderManager::Use() {
    glUseProgram(shaderProgram);
}

void ShaderManager::SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix) {
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }
    else {
        std::cerr << "Uniform " << name << " not found!" << std::endl;
    }
}

void ShaderManager::SetUniform3f(const std::string& name, const glm::vec3& value) {
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location != -1) {
        glUniform3fv(location, 1, &value[0]);
    }
    else {
        std::cerr << "Uniform " << name << " not found!" << std::endl;
    }
}
void ShaderManager::SetUniform1f(const std::string& name, float value) {
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' not found!" << std::endl;
    }
    else {
        glUniform1f(location, value);
    }
}

void ShaderManager::SetUniform4f(const std::string& name, const glm::vec4& value) {
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location != -1) {
        glUniform4fv(location, 1, &value[0]);
    }
    else {
        std::cerr << "Uniform " << name << " not found!" << std::endl;
    }
}

void ShaderManager::SetUniform1i(const std::string& name, int value) {
    // Get the location of the uniform variable in the shader program
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform " << name << " not found!" << std::endl;
    }
    // Set the uniform value
    glUniform1i(location, value);
}

GLuint ShaderManager::CompileShader(GLenum type, const std::string& source) {
    GLuint shaderID = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        glGetShaderInfoLog(shaderID, length, &length, log);
        std::cerr << "Shader compilation error: " << log << std::endl;
        delete[] log;
    }

    return shaderID;
}

void ShaderManager::LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLint length;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        glGetProgramInfoLog(shaderProgram, length, &length, log);
        std::cerr << "Program linking error: " << log << std::endl;
        delete[] log;
    }

    // After linking, shaders are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


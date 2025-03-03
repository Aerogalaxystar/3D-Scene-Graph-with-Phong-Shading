#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>  // Make sure GLEW is initialized first.
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

class ShaderManager {
public:
    GLuint shaderProgram;

    // Constructor that accepts shader source code
    ShaderManager(const std::string& vertexSource, const std::string& fragmentSource);
    ~ShaderManager();

    void Use();
    void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
    void SetUniform3f(const std::string& name, const glm::vec3& value);
    void SetUniform4f(const std::string& name, const glm::vec4& value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int value);

private:
    GLuint CompileShader(GLenum type, const std::string& source);
    void LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif
// !SHADERMANAGER_H


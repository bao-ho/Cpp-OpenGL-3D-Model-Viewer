#include <GL/glew.h>
#include <tuple>
#include <fstream>
#include <iostream>
#include "Shader.h"

static std::tuple<std::string, std::string> parseShader(const std::string& fileName)
{
    std::string line;
    std::string shaderText[2];
    unsigned int shaderType = 0;
    std::ifstream inputFileStream(fileName);
    if (inputFileStream.is_open())
    {
        while (std::getline(inputFileStream, line))
        {
            if (line.find("shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    shaderType = 0;
                else if (line.find("fragment") != std::string::npos)
                    shaderType = 1;
            }
            else
                shaderText[shaderType] += line + "\n";
        }
    }
    inputFileStream.close();
    return { shaderText[0], shaderText[1] };
}
static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
unsigned int createShader(const std::string& fileName)
{
    auto [vertexShader, fragmentShader] = parseShader(fileName);
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
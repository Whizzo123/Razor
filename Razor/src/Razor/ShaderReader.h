#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

enum ShaderType {VERTEX, FRAGMENT};

class ShaderReader
{
public:
    ShaderReader();
    ~ShaderReader();

    
    static unsigned int CreateShader(std::string shaderName, ShaderType type);
    static unsigned int CreateShaderProgram(std::string vertexShader, std::string fragmentShader);
    

private:
    static std::string ReadInShader(std::string shaderName);
};

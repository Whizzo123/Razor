#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Core.h"

enum ShaderType {VERTEX, FRAGMENT};

namespace Razor
{
    class ShaderReader
    {
    public:
        ShaderReader();
        ~ShaderReader();

        static Scope<unsigned int> CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);

    private:
        static Scope<unsigned int> CreateShader(const std::string& shaderName, ShaderType type);
        static std::string ReadInShader(const std::string& shaderName);
    };
}

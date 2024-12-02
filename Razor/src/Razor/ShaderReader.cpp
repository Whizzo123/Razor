#include "ShaderReader.h"
#include "Log.h"



ShaderReader::ShaderReader() = default;
ShaderReader::~ShaderReader() = default;

std::string ShaderReader::ReadInShader(std::string shaderName)
{
    std::ifstream t("resources/shaders/" + shaderName);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}


unsigned int ShaderReader::CreateShader(std::string shaderName, ShaderType type)
{
    std::string temp = ReadInShader(shaderName);

    unsigned int shader = -1;

    if (temp.empty())
    {
        RZ_CORE_ERROR("ERROR::SHADER:: {0} ::COMPILATION::FAILED no file found or empty file at {1}", type, shaderName );
        return shader;
    }
	
    const char* source = temp.c_str();
	
    switch(type)
    {
        case VERTEX:
            {
                shader = glCreateShader(GL_VERTEX_SHADER);
                break;
            }
        case FRAGMENT:
            {
                shader = glCreateShader(GL_FRAGMENT_SHADER);
                break;
            }
    }
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
	
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::"<< type <<"::COMPILATION::FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned ShaderReader::CreateShaderProgram(std::string vertexShaderFile, std::string fragmentShaderFile)
{
    unsigned int vertexShader = CreateShader(vertexShaderFile, VERTEX);
    unsigned int fragmentShader = CreateShader(fragmentShaderFile, FRAGMENT);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING::FAILED" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}



#include "ShaderReader.h"
#include "Log.h"
#include "Engine.h"
#include "Renderer/Shaders/Shader.h"

namespace Razor
{

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
            RZ_CORE_ERROR("ERROR::SHADER:: {0} ::COMPILATION::FAILED no file found or empty file at {1}", type, shaderName);
            return shader;
        }

        const char* source = temp.c_str();

        Ref<IRenderer> Renderer = Engine::Get().GetRenderer();

        switch (type)
        {
        case VERTEX:
        {
            shader = Renderer->CreateShader(EShader::VERTEX);
            break;
        }
        case FRAGMENT:
        {
            shader = Renderer->CreateShader(EShader::FRAGMENT);
            break;
        }
        }
        Renderer->SetShaderSource(shader, 1, &source, nullptr);
        Renderer->CompileShader(shader);

        int success;
        char infoLog[512];
        Renderer->GetShaderStatusParam(shader, EStatusParam::COMPILE, &success);
        if (!success)
        {
            Renderer->GetShaderLog(shader, 512, nullptr, infoLog);
            RZ_CORE_ERROR("ERROR::SHADER::{0}::COMPILATION::FAILED\n{1}", type, infoLog);
        }

        return shader;
    }

    unsigned ShaderReader::CreateShaderProgram(std::string vertexShaderFile, std::string fragmentShaderFile)
    {
        Ref<IRenderer> Renderer = Engine::Get().GetRenderer();

        unsigned int vertexShader = CreateShader(vertexShaderFile, VERTEX);
        unsigned int fragmentShader = CreateShader(fragmentShaderFile, FRAGMENT);

        unsigned int shaderProgram;
        shaderProgram = Renderer->CreateShaderProgram();

        Renderer->AttachShader(shaderProgram, vertexShader);
        Renderer->AttachShader(shaderProgram, fragmentShader);
        Renderer->LinkShaderProgram(shaderProgram);

        int success;
        Renderer->GetShaderProgramStatusParam(shaderProgram, EProgramStatusParam::LINK, &success);
        if (!success)
        {
            char infoLog[512];
            Renderer->GetShaderProgramLog(shaderProgram, 512, nullptr, infoLog);
            RZ_CORE_ERROR("ERROR::PROGRAM::LINKING::FAILED\n{0}", infoLog);
        }

        Renderer->DeleteShader(vertexShader);
        Renderer->DeleteShader(fragmentShader);

        return shaderProgram;
    }

}

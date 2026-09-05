#include "ShaderReader.h"
#include "Log.h"
#include "Engine.h"
#include "Renderer/Shaders/Shader.h"

namespace Razor
{

    ShaderReader::ShaderReader() = default;
    ShaderReader::~ShaderReader() = default;

    std::string ShaderReader::ReadInShader(const std::string& shaderName)
    {
        std::ifstream t("Edge/resources/shaders/" + shaderName);
        std::stringstream buffer;
        buffer << t.rdbuf();
        return buffer.str();
    }


    Scope<unsigned int> ShaderReader::CreateShader(const std::string& shaderName, ShaderType type)
    {
        std::string shaderFileContent = ReadInShader(shaderName);

        Scope<unsigned int> shader = nullptr;

        if (shaderFileContent.empty())
        {
            RZ_CORE_ERROR("ERROR::SHADER:: {0} ::COMPILATION::FAILED no file found or empty file at {1}", static_cast<int>(type), shaderName);
            return shader;
        }

        const char* source = shaderFileContent.c_str();

        Ref<IRenderer> Renderer = Engine::Get().GetRenderer();

        switch (type)
        {
        case VERTEX:
        {
            shader = CreateScope<unsigned int>(Renderer->CreateShader(EShader::VERTEX));
            break;
        }
        case FRAGMENT:
        {
            shader = CreateScope<unsigned int>(Renderer->CreateShader(EShader::FRAGMENT));
            break;
        }
        }
        Renderer->SetShaderSource(*shader, 1, &source, nullptr);
        Renderer->CompileShader(*shader);

        int success;
        char infoLog[512];
        Renderer->GetShaderStatusParam(*shader, EStatusParam::COMPILE, &success);
        if (!success)
        {
            Renderer->GetShaderLog(*shader, 512, nullptr, infoLog);
            RZ_CORE_ERROR("ERROR::SHADER::{0}::COMPILATION::FAILED\n{1}", static_cast<int>(type), infoLog);
        }

        return shader;
    }

    Scope<unsigned int> ShaderReader::CreateShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
    {
        Ref<IRenderer> Renderer = Engine::Get().GetRenderer();

        Scope<unsigned int> vertexShader = CreateShader(vertexShaderFile, VERTEX);
        Scope<unsigned int> fragmentShader = CreateShader(fragmentShaderFile, FRAGMENT);
        Scope<unsigned int> shaderProgram = nullptr;

        if (!vertexShader || !fragmentShader) {
            RZ_CORE_ERROR("Failed to create shader program one of the shaders were null");
            return shaderProgram;
        }

        
        shaderProgram = CreateScope<unsigned int>(Renderer->CreateShaderProgram());

        Renderer->AttachShader(*shaderProgram, *vertexShader);
        Renderer->AttachShader(*shaderProgram, *fragmentShader);
        Renderer->LinkShaderProgram(*shaderProgram);

        int success;
        Renderer->GetShaderProgramStatusParam(*shaderProgram, EProgramStatusParam::LINK, &success);
        if (!success)
        {
            char infoLog[512];
            Renderer->GetShaderProgramLog(*shaderProgram, 512, nullptr, infoLog);
            RZ_CORE_ERROR("ERROR::PROGRAM::LINKING::FAILED\n{0}", infoLog);
        }

        Renderer->DeleteShader(*vertexShader);
        Renderer->DeleteShader(*fragmentShader);

        return shaderProgram;
    }

}

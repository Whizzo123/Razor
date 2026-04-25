#include "OpenGLRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Log.h"
#include "../../Platform/OpenGL/OpenGLWindowProvider.h"
#include "../Assert.h"
#include "OpenGL/GLFramebuffer.h"
#include "../Window.h"
#include "Debug/DebugLine.h"
#include "Debug/DebugTriangle.h"

namespace Razor
{

    void OpenGLRenderer::InitRendererAPI()
    {   
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

        if(!glfwInit()) {
            std::string buffer(512, '*');
            const char* buffer_ptr = buffer.c_str();
            int ret = glfwGetError(&buffer_ptr);
            RZ_CORE_ERROR("Failed to init GLFW {0}", buffer);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        StencilActionTranslation = std::unordered_map<EStencilAction, GLenum> 
        {
            {EStencilAction::KEEP, GL_KEEP},
            {EStencilAction::REPLACE, GL_REPLACE}
        };

        StencilFuncTranslation = std::unordered_map<EStencilFunc, GLenum>
        {
            {EStencilFunc::ALWAYS, GL_ALWAYS}
        };

        ShaderTranslation = std::unordered_map<EShader, GLenum>
        {
            {EShader::VERTEX, GL_VERTEX_SHADER},
            {EShader::FRAGMENT, GL_FRAGMENT_SHADER}
        };

        StatusParamTranslation = std::unordered_map<EStatusParam, GLenum>
        {
            {EStatusParam::COMPILE, GL_COMPILE_STATUS}
        };

        ProgramStatusParamTranslation = std::unordered_map<EProgramStatusParam, GLenum>
        {
            {EProgramStatusParam::LINK, GL_LINK_STATUS}
        };

        TextureTypeTranslation = std::unordered_map<ETextureType, GLenum>
        {
            {ETextureType::TEXTURE_2D, GL_TEXTURE_2D}
        };

        PixelDataFormatTranslation = std::unordered_map<EPixelDataFormat, GLenum>
        {
            {EPixelDataFormat::RGBA, GL_RGBA}
        };

        PixelDataTypeTranslation = std::unordered_map<EPixelDataType, GLenum>
        {
            {EPixelDataType::UNSIGNED_BYTE, GL_UNSIGNED_BYTE}
        };
    }

    void OpenGLRenderer::PollForEvents()
    {
        glfwPollEvents();
    }

    void OpenGLRenderer::TerminateRendererAPI()
    {
        glfwTerminate();
    }

    void OpenGLRenderer::EnableDepthTesting(bool bEnable)
    {
        bEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRenderer::EnableStencilTesting(bool bEnable)
    {
        bEnable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
    }

    void OpenGLRenderer::SetStencilOperation(EStencilAction StencilFailAction, EStencilAction DepthFailAction, EStencilAction SDPassAction)
    {
        glStencilOp(Translate(StencilFailAction), Translate(DepthFailAction), Translate(SDPassAction));
    }

    void OpenGLRenderer::UseShader(uint32_t ShaderProgramId)
    {
        glUseProgram(ShaderProgramId);
    }

    GLenum OpenGLRenderer::Translate(EStencilAction Action)
    {
        RZ_CORE_ASSERT(StencilActionTranslation.find(Action) != StencilActionTranslation.end(), "Can't translate this action");
        return StencilActionTranslation[Action];
    }

    GLenum OpenGLRenderer::Translate(EStencilFunc Action)
    {
        RZ_CORE_ASSERT(StencilFuncTranslation.find(Action) != StencilFuncTranslation.end(), "Can't translate this function");
        return StencilFuncTranslation[Action];
    }

    void OpenGLRenderer::SetStencilFunction(EStencilFunc Function, unsigned int Ref, unsigned int Mask)
    {
        glStencilFunc(Translate(Function), Ref, Mask);
    }

    void OpenGLRenderer::EnableStencilWriting(bool bEnable)
    {
        bEnable ? glStencilMask(0xFF) : glStencilMask(0x00);
    }

    CameraInfo OpenGLRenderer::GetCameraInfo()
    {
        // Swap these out for actual grabbed values from potentially window class
        const int SCREEN_WIDTH = 800;
        const int SCREEN_HEIGHT = 600;

        CameraInfo Info;
        glm::mat4 View = glm::mat4(1.0f);
        glm::mat4 Projection = glm::mat4(1.0f);
        Projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        View = glm::lookAt(RendererCamera.CameraPos, RendererCamera.CameraPos + RendererCamera.CameraFront, RendererCamera.CameraUp);
        Info.CameraView = View;
        Info.CameraProjection = Projection;
        Info.CameraPosition = RendererCamera.CameraPos;
        return Info;
    }

    void OpenGLRenderer::DrawMesh(RendererInfo Info)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindVertexArray(Info.ObjMesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Info.ObjMesh.Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void OpenGLRenderer::DrawLine(const DebugLine& line)
    {
        unsigned int VBO, VAO;
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
        Vector3 vertices[] = {line.a, line.b};
        glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(Vector3), &vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINES, 0, 2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

    void OpenGLRenderer::DrawTriangle(const DebugTriangle& triangle)
    {
        unsigned int VBO, VAO;
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        Vector3 vertices[] = { triangle.v1, triangle.v2, triangle.v3 };
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vector3), &vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

    void OpenGLRenderer::ClearBuffer()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void OpenGLRenderer::BindFrameBuffer(uint32_t BufferIndex)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, BufferIndex);
    }

    Ref<Framebuffer> OpenGLRenderer::CreateFrameBuffer(uint32_t Width, uint32_t Height)
    {
        Ref<Framebuffer> Buffer = CreateRef<GLFramebuffer>();
        Buffer->Refresh(Width, Height);
        return Buffer;
    }

    void OpenGLRenderer::SwapBuffer(Window& window)
    {
        glfwSwapBuffers(std::dynamic_pointer_cast<OpenGLWindowProvider>(window.GetWindowProvider())->GetPlatformWindowPtr());
    }

    void OpenGLRenderer::ReadPixels(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, float* OutPixels, unsigned int Buffer)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(X, Y, Width, Height, GL_RGB, GL_FLOAT, OutPixels);
    }

    void OpenGLRenderer::BackupContext()
    {
        GLFWBackupContext = glfwGetCurrentContext();
    }

    void OpenGLRenderer::ResetCurrentContext()
    {
        if(GLFWBackupContext != nullptr)
        { 
            glfwMakeContextCurrent(GLFWBackupContext);
        }
        else
        {
            RZ_CORE_ERROR("Failed to reset current context 'GLFWBackupContext' == nullptr");
        }
    }

    void* OpenGLRenderer::GetSceneRenderedToTexture()
    {
        return (void*)(GLuint)SceneTexture;
    }

    void OpenGLRenderer::SetFrameBufferTextureForScene(unsigned int InSceneTexture)
    {
        SceneTexture = InSceneTexture;
    }

    //TODO potentially remove this do we actually use this?
    void OpenGLRenderer::ResizeFramebuffers(unsigned int Width, unsigned int Height)
    {
        //There are things that depend on these framebuffers so how do we notify them that the framebuffers have been regenerated and they need to grab the new ones
        //How do we know what framebuffers to regenerate?
        //Do we keep a list of these framebuffers and there relevant textures a new type?
    }
    
    void OpenGLRenderer::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
    {
        glViewport(X, Y, Width, Height);
    }

    uint32_t OpenGLRenderer::CreateShader(EShader ShaderType)
    {
        return glCreateShader(ShaderTranslation[ShaderType]);
    }
    void OpenGLRenderer::SetShaderSource(uint32_t ShaderHandle, int32_t Count, const char* const* String, const int* Length)
    {
        glShaderSource(ShaderHandle, Count, String, Length);
    }

    void OpenGLRenderer::CompileShader(uint32_t ShaderHandle)
    {
        glCompileShader(ShaderHandle);
    }
    void OpenGLRenderer::GetShaderStatusParam(uint32_t ShaderHandle, EStatusParam Param, int* SuccessCode)
    {
        glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, SuccessCode);
    }
    void OpenGLRenderer::GetShaderLog(uint32_t ShaderHandle, int32_t MaxLength, int32_t* Length, char* Log)
    {
        glGetShaderInfoLog(ShaderHandle, MaxLength, Length, Log);
    }
    uint32_t OpenGLRenderer::CreateShaderProgram()
    {
        return glCreateProgram();
    }
    void OpenGLRenderer::AttachShader(uint32_t ShaderProgramHandle, uint32_t ShaderHandle)
    {
        glAttachShader(ShaderProgramHandle, ShaderHandle);
    }

    void OpenGLRenderer::LinkShaderProgram(uint32_t ShaderProgramHandle)
    {
        glLinkProgram(ShaderProgramHandle);
    }

    void OpenGLRenderer::GetShaderProgramStatusParam(uint32_t ShaderProgramHandle, EProgramStatusParam Param, int* SuccessCode)
    {
        glGetProgramiv(ShaderProgramHandle, ProgramStatusParamTranslation[Param], SuccessCode);
    }

    void OpenGLRenderer::GetShaderProgramLog(uint32_t ShaderProgramHandle, int32_t MaxLength, int32_t* Length, char* Log)
    {
        glGetProgramInfoLog(ShaderProgramHandle, MaxLength, Length, Log);
    }

    void OpenGLRenderer::DeleteShader(uint32_t ShaderHandle)
    {
        glDeleteShader(ShaderHandle);
    }
    void OpenGLRenderer::GenerateTextures(int32_t NumberOfTextures, uint32_t* Textures)
    {
        glGenTextures(NumberOfTextures, Textures);
    }
    void OpenGLRenderer::BindTexture(ETextureType TextureType, uint32_t TextureHandle)
    {
        glBindTexture(TextureTypeTranslation[TextureType], TextureHandle);
    }
    void OpenGLRenderer::WriteTexture2dData(ETextureType TextureType, int32_t LevelOfDetail, EPixelDataFormat NumberOfColorComponents, int32_t Width, int32_t Height, int32_t Border, EPixelDataFormat DataFormat, EPixelDataType DataType, const void* Data)
    {
        glTexImage2D(TextureTypeTranslation[TextureType], LevelOfDetail, PixelDataFormatTranslation[NumberOfColorComponents], Width, Height, Border, PixelDataFormatTranslation[DataFormat], PixelDataTypeTranslation[DataType], Data);
    }
    void OpenGLRenderer::GenerateMipmap(ETextureType TextureType)
    {
        glGenerateMipmap(TextureTypeTranslation[TextureType]);
    }
}
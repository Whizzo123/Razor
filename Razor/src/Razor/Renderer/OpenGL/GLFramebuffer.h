#pragma once
#include "../Framebuffer.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace Razor
{
    struct GLFramebuffer : public Framebuffer
    {
        void CreateFramebuffer() override
        {
            glGenFramebuffers(1, &ID);
            glBindFramebuffer(GL_FRAMEBUFFER, ID);
            glGenTextures(1, &Texture);
            glBindTexture(GL_TEXTURE_2D, Texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);
            glGenRenderbuffers(1, &DepthID);
            glBindRenderbuffer(GL_RENDERBUFFER, DepthID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthID);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void Refresh(uint32_t InWidth, uint32_t InHeight) override
        {
            if ((InWidth != Width || InHeight != Height) && (InHeight != 0 && InWidth != 0))
            {
                RZ_CORE_WARN("Invalidating current framebuffer old({0},{1}) vs new({2},{3})", Width, Height, InWidth, InHeight);
                Width = InWidth;
                Height = InHeight;
                Invalidate();
            }
        }

        void Invalidate() override
        {
            glDeleteFramebuffers(1, &ID);
            glDeleteTextures(1, &Texture);
            ID, Texture = 0;
            CreateFramebuffer();
        }

        uint32_t GetID() override { return ID; }
        uint32_t GetTexture() override { return Texture; }
        uint32_t GetRenderID() override { return DepthID; }
        uint32_t GetWidth() override { return Width; }
        uint32_t GetHeight() override { return Height; }

    private:
        uint32_t ID;
        uint32_t Texture;
        uint32_t DepthID;
        unsigned int Width;
        unsigned int Height;
    };
}

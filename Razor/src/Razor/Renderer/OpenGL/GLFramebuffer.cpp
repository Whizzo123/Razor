
#include "GLFramebuffer.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "../../Log.h"

namespace Razor
{
    void GLFramebuffer::CreateFramebuffer()
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
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            RZ_CORE_ERROR("Framebuffer failed to create is not complete!");
            glDeleteFramebuffers(1, &ID);
            glDeleteTextures(1, &Texture);
        }
        else
        {
            RZ_CORE_INFO("Framebuffer created successfully");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFramebuffer::Refresh(uint32_t InWidth, uint32_t InHeight)
    {
        if ((InWidth != Width || InHeight != Height) && (InHeight != 0 && InWidth != 0))
        {
            Width = InWidth;
            Height = InHeight;
            Invalidate();
        }
    }

    void GLFramebuffer::Invalidate()
    {
        glDeleteFramebuffers(1, &ID);
        glDeleteTextures(1, &Texture);
        ID, Texture = 0;
        CreateFramebuffer();
    }
}
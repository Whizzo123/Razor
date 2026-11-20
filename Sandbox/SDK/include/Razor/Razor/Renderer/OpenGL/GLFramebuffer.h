#pragma once
#include "../Framebuffer.h"

namespace Razor
{
    struct RAZOR_API GLFramebuffer : public Framebuffer
    {
        ~GLFramebuffer() override = default;

        void CreateFramebuffer() override;

        void Refresh(uint32_t InWidth, uint32_t InHeight) override;

        void Invalidate() override;

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

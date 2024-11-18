#pragma once

namespace Razor
{

	struct Framebuffer
	{
		virtual void CreateFramebuffer() = 0;
		virtual void Invalidate() = 0;
		virtual void Refresh(uint32_t InWidth, uint32_t InHeight) = 0;
		virtual uint32_t GetID() = 0;
		virtual uint32_t GetTexture() = 0;
		virtual uint32_t GetRenderID() = 0;
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
	};
}
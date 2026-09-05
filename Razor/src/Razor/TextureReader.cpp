#include "TextureReader.h"
#include "stb_image.h"
#include "Engine.h"
#include "Renderer/IRenderer.h"
#include "Log.h"

namespace Razor
{
    unsigned TextureReader::CreateTexture(const std::string& fileName)
    {
        Ref<IRenderer> Renderer = Engine::Get().GetRenderer();
        int width, height, nrChannels;
        std::string fullPath = ("resources/textures/" + fileName);
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
        //TODO there is no way to tell from this if texture actually worked or not we are just defaulting to whatever texture is living in slot 1 not ideal possibly optional here
        unsigned int texture = 0;
        if (data)
        {
            Renderer->GenerateTextures(1, &texture);

            Renderer->BindTexture(ETextureType::TEXTURE_2D, texture);
            Renderer->WriteTexture2dData(ETextureType::TEXTURE_2D, 0, EPixelDataFormat::RGBA, width, height, 0, EPixelDataFormat::RGBA, EPixelDataType::UNSIGNED_BYTE, data);

            Renderer->GenerateMipmap(ETextureType::TEXTURE_2D);
        }
        else
        {
            RZ_CORE_ERROR("TextureReader::CreateTexture -> Failed to load texture");
        }
        stbi_image_free(data);

        return texture;
    }
}

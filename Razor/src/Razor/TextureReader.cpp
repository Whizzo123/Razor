#include "TextureReader.h"
#include "stb_image.h"
#include "Engine.h"
#include "Renderer/IRenderer.h"

namespace Razor
{
    unsigned TextureReader::CreateTexture(std::string fileName)
    {
        Ref<IRenderer> Renderer = Engine::Get().GetRenderer();
        int width, height, nrChannels;
        std::string temp = ("resources/textures/" + fileName);
        const char* filePath = temp.c_str();
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
        unsigned int texture;
        if (data)
        {
            Renderer->GenerateTextures(1, &texture);

            Renderer->BindTexture(ETextureType::TEXTURE_2D, texture);
            Renderer->WriteTexture2dData(ETextureType::TEXTURE_2D, 0, EPixelDataFormat::RGBA, width, height, 0, EPixelDataFormat::RGBA, EPixelDataType::UNSIGNED_BYTE, data);

            Renderer->GenerateMipmap(ETextureType::TEXTURE_2D);
        }
        else
        {
            std::cout << "ERROR::FAILED::TO::LOAD::TEXTURE" << std::endl;
        }
        stbi_image_free(data);

        return texture;
    }
}

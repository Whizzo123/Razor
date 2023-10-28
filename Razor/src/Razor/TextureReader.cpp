#include "TextureReader.h"

#include "stb_image.h"

unsigned TextureReader::CreateTexture(std::string fileName)
{

    int width, height, nrChannels;
    std::string temp = ("resources/textures/" + fileName);
    const char* filePath = temp.c_str();
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    unsigned int texture;
    if(data)
    {
        
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::FAILED::TO::LOAD::TEXTURE" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

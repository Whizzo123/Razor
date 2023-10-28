#pragma once
#include <string>

#include <iostream>
#include <glad/glad.h>

class TextureReader
{
public:
    static unsigned int CreateTexture(std::string fileName);
    
};

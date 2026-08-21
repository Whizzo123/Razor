#pragma once

#include "Character.h"
#include "../../Core.h"
#include <map>

namespace Razor
{
    class RAZOR_API Font
    {
    public:
        Font();
        Font(std::map<char, Character> characters);
        Character getCharacter(char character);
    private:
        std::map<char, Character> _mCharacters;
    };
}
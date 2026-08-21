#include "Font.h"

namespace Razor
{
    Font::Font()
    {

    }

    Font::Font(std::map<char, Character> characters)
        : _mCharacters(characters)
    {

    }

    Character Font::getCharacter(char character)
    {
        return _mCharacters[character];
    }
}
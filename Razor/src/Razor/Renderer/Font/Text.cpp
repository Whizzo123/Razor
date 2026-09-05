#include "Text.h"
#include "../../Engine.h"

namespace Razor
{
    Text::Text(Shader& shader, const std::string& text) :
        mScale(1.0f), mColor({1.0f, 1.0f, 1.0f}), mShader(shader), _mText(text)
    {
        Engine::Get().GetRenderer()->CreateText(*this);
    }

    Text::~Text()
    {

    }
}
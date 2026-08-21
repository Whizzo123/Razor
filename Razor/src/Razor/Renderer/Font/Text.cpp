#include "Text.h"
#include "../../Engine.h"

namespace Razor
{
    Text::Text(Shader& shader, const std::string& text) :
        mShader(shader), _mText(text), mColor({1.0f, 1.0f, 1.0f}), mScale(1.0f)
    {
        Engine::Get().GetRenderer()->CreateText(*this);
    }

    Text::~Text()
    {

    }
}
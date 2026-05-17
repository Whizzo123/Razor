#pragma once

#include <string>
#include "../../Utils/Vector.h"
#include "../Shaders/Shader.h"
#include <map>
namespace Razor
{
    class RAZOR_API Text
    {
    public:
        Text(Shader& shader, const std::string& text);
        ~Text();

        const std::string& GetText() { return _mText; }
        void SetText(const std::string& text) { _mText = text; }

        unsigned int mVao;
        unsigned int mVbo;
        Vector2 mPosition;
        float mScale;
        Vector3 mColor;
        Shader& mShader;
        AssetKey mFontKey;
    private:
        std::string _mText;
    };
}
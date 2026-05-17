#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include "Character.h"
#include <string>
#include "../../Core.h"

namespace Razor 
{
    class FontLoader 
    {
    public:
        FontLoader();
        ~FontLoader();

        std::map<char, Character> LoadFontMap(std::string& path);
    private:
        void Init();
        void LoadFace(std::string& path, FT_Face& face, FT_Long index);
        void LoadFace(FT_Byte* buffer, FT_Long size, FT_Face& face, FT_Long index);
        void SetPixelSize(FT_Face& face, FT_UInt width, FT_UInt height);
        FT_UInt GetGlyphIndex(FT_Face& face, FT_ULong charcode);
        void LoadGlyph(FT_Face& face, FT_UInt index, FT_Int32 flags);
        Ref<FT_Bitmap> RenderGlyph(FT_Face& face);
        void Close();
        
    private:
        FT_Library _mLibrary;
    };
}
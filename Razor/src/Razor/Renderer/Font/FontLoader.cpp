#include "FontLoader.h"
#include "../../Log.h"
#include "../IRenderer.h"
#include "../Textures/Bitmap.h"
#include "../../Engine.h"

namespace Razor 
{
    FontLoader::FontLoader()
    {
        Init();
    }

    FontLoader::~FontLoader()
    {
        Close();
    }

    void FontLoader::Init() 
    {
        FT_Error error = FT_Init_FreeType(&_mLibrary);
        if (error)
        {
            RZ_CORE_ERROR("FontLoader::Init -> Failed to initialise library code: {0}", error);
        }
        RZ_CORE_INFO("FontLoader::Init -> Successfully initialised library");
    }

    void FontLoader::LoadFace(std::string& path, FT_Face& face, FT_Long index)
    {
        FT_Error error;
        error = FT_New_Face(_mLibrary, path.c_str(), index, &face);
        if (error == FT_Err_Unknown_File_Format)
        {
            RZ_CORE_ERROR("FontLoader::LoadFace -> Failed to load face for {0} unsuppored file format", path);
        }
        else if ( error )
        {
            RZ_CORE_ERROR("FontLoader::LoadFace -> Failed to load face for {0} error code: {1}", path, error);
        }
    }

    void FontLoader::LoadFace(FT_Byte* buffer, FT_Long size, FT_Face& face, FT_Long index)
    {
        FT_Error error;
        error = FT_New_Memory_Face(_mLibrary, buffer, size, index, &face);
        if ( error )
        {
            RZ_CORE_ERROR("FontLoader::LoadFace -> Failed to load face from memory code: {0}", error);
        }
    }

    void FontLoader::SetPixelSize(FT_Face& face, FT_UInt width, FT_UInt height)
    {
        FT_Error error;
        error = FT_Set_Pixel_Sizes(face, width, height);
        if (error)
        {
            RZ_CORE_ERROR("FontLoader::SetPixelSize -> Failed to set pixel size for face code: {0}", error);
        }
    }

    FT_UInt FontLoader::GetGlyphIndex(FT_Face& face, FT_ULong charcode)
    {
        FT_UInt glyph_index = FT_Get_Char_Index(face, charcode);
        return glyph_index;
    }

    void FontLoader::LoadGlyph(FT_Face& face, FT_UInt index, FT_Int32 flags)
    {
        FT_Error error;
        error = FT_Load_Glyph(face, index, flags);
        if (error)
        {
            RZ_CORE_ERROR("FontLoader::LoadGlyph -> Failed to load glyph index {0} for face code: {1}", index, error);
        }
    }

    Ref<FT_Bitmap> FontLoader::RenderGlyph(FT_Face& face)
    {
        FT_Error error;
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if (error)
        {
            RZ_CORE_ERROR("FontLoader::RenderGlyph -> Failed to render glyph for face code: {0}", error);
            return nullptr;
        }
        return CreateRef<FT_Bitmap>(face->glyph->bitmap);
    }

    std::map<char, Character> FontLoader::LoadFontMap(std::string& path)
    {
        Ref<IRenderer> renderer = Engine::Get().GetRenderer();
        std::map<char, Character> Characters;
        renderer->DisableByteAlignment();
        FT_Face face;
        LoadFace(path, face, 0);
        SetPixelSize(face, 0, 48);
        for (unsigned char c = 0; c < 128; c++)
        {
            FT_UInt index = GetGlyphIndex(face, c);
            if (index == 0)
            {
                RZ_CORE_WARN("Character {0} not in font", static_cast<char>(c));
                continue;
            }
            LoadGlyph(face, index, 0);
            Ref<FT_Bitmap> ftBitmap = RenderGlyph(face);

            if (ftBitmap->buffer)
            {
                Bitmap bitmap;
                bitmap.width = ftBitmap->width;
                bitmap.rows = ftBitmap->rows;
                bitmap.buffer = CreateScope<unsigned char[]>(bitmap.width * bitmap.rows);
                std::memcpy(bitmap.buffer.get(), ftBitmap->buffer, bitmap.width * bitmap.rows);

                unsigned int texture;
                renderer->GenerateTextures(1, &texture);
                renderer->BindTexture(ETextureType::TEXTURE_2D, texture);
                renderer->WriteTextureBitmapData(bitmap);
                renderer->SetTextureParameterInt(ETextureType::TEXTURE_2D, ETextureOption::WRAP_S, ETextureValue::CLAMP_TO_EDGE);
                renderer->SetTextureParameterInt(ETextureType::TEXTURE_2D, ETextureOption::WRAP_T, ETextureValue::CLAMP_TO_EDGE);
                renderer->SetTextureParameterInt(ETextureType::TEXTURE_2D, ETextureOption::MIN_FILTER, ETextureValue::LINEAR);
                renderer->SetTextureParameterInt(ETextureType::TEXTURE_2D, ETextureOption::MAG_FILTER, ETextureValue::LINEAR);
                Character character = 
                {
                    texture,
                    Vector2u{bitmap.width, bitmap.rows},
                    Vector2i{face->glyph->bitmap_left, face->glyph->bitmap_top},
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair(c, character));
                renderer->BindTexture(ETextureType::TEXTURE_2D, 0);
            }
        }
        renderer->SetByteAlignment(4);
        FT_Done_Face(face);
        return Characters;
    }

    void FontLoader::Close()
    {
        FT_Done_FreeType(_mLibrary);
    }
}
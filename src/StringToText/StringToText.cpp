#include "StringToText/StringToText.h"

void StringToTextConverter::resizeBuffer(unsigned int newStride, int unsigned newRows) {
    std::vector<unsigned char> newcharbitmap;
    newcharbitmap.resize(newStride * newRows);

    for(int r = 0; r < rows; ++r) {
        int oldbitmapoffset = r * stride;
        int newbitmapoffset = r * newStride;

        memcpy(newcharbitmap.data() + newbitmapoffset, charbitmap.data() + oldbitmapoffset, stride);
    }
    stride = newStride;
    rows = newRows;

    charbitmap = newcharbitmap;
}

void StringToTextConverter::renderCharToBitmap(FT_Bitmap bitmap, unsigned int offsetX, unsigned int offsetY) {
    for(int r = 0; r < rows; ++r) {
        if(r < offsetY || r >= offsetY + bitmap.rows) {
            continue;
        }
        int glyphbitmapoffset = (r - offsetY) * bitmap.width * sizeof(unsigned char);
        int bitmapoffset = r * stride + offsetX;

        memcpy(charbitmap.data() + bitmapoffset, bitmap.buffer + glyphbitmapoffset, bitmap.width * sizeof(unsigned char));
    }
}

void StringToTextConverter::renderCharacter(FT_Bitmap bitmap, unsigned int offsetX, unsigned int offsetY) {
    if(stride < (offsetX + bitmap.width * sizeof(unsigned char)) || rows < (offsetY + bitmap.rows)) {
        resizeBuffer(std::max(stride, offsetX + bitmap.width), std::max(rows, offsetY + bitmap.rows));
    }

    renderCharToBitmap(bitmap, offsetX, offsetY);
}

StringToTextConverter::StringToTextConverter(std::string _fontFile) : fontFile(_fontFile) {
    error = FT_Init_FreeType(&library);

    if(error) {
        throw std::runtime_error("an error occured during freetype2 library initialization");
    }

    error = FT_New_Face(library, fontFile.data(), 0, &face);

    if(error == FT_Err_Unknown_File_Format) {
        throw std::runtime_error("the font comic_sans.ttf is unsupported");
    }else if(error) {
        throw std::runtime_error("font file could not be open/read, or is otherwise broken.");
    }

    error = FT_Set_Char_Size(
          face,    /* handle to face object           */
          0,       /* char_width in 1/64th of points  */
          16*64,   /* char_height in 1/64th of points */
          300,     /* horizontal device resolution    */
          300 );   /* vertical device resolution      */

    if(error) {
        throw std::runtime_error("failed to set face char size");
    }
}

TextBitmap StringToTextConverter::getTextFromString(std::string str) {
    for (int n = 0; n < str.size(); n++) {
        if(str[n] == '\n') {
            pen_y += face->size->metrics.height >> 6;
            pen_x = 0;
            continue;
        }
        
        FT_UInt  glyph_index;

        /* retrieve glyph index from character code */
        glyph_index = FT_Get_Char_Index(face, str[n]);

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (error)
            continue;  /* ignore errors */

        /* convert to an anti-aliased bitmap */
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL);
        if ( error )
            continue;

        /* now, draw to our target surface */
        FT_Bitmap bitmap = face->glyph->bitmap;

        if(pen_x == -1 || pen_y == -1) {
            pen_x = 0;
            pen_y = face->size->metrics.ascender >> 6;
        }

        renderCharacter(bitmap, pen_x + face->glyph->bitmap_left, pen_y - face->glyph->bitmap_top);

        /* increment pen position */
        pen_x += face->glyph->advance.x >> 6;
        pen_y += face->glyph->advance.y >> 6; /* not useful for now */
    }

    TextBitmap text = TextBitmap();
    text.bitmap = charbitmap;
    text.stride = stride;
    text.rows = rows;

    charbitmap = std::vector<unsigned char>();
    stride = 0;
    rows = 0;
    pen_x = -1;
    pen_y = -1;

    return text;
}
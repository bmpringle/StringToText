#ifndef STRINGTOTEXT_H
#define STRINGTOTEXT_H

#include "ft2build.h"
#include FT_FREETYPE_H

#include <stdexcept>

#include <vector>

#include <iostream>

struct TextBitmap {
    std::vector<unsigned char> bitmap;
    unsigned int stride;
    unsigned int rows;
};

class StringToTextConverter {
    public:
        StringToTextConverter(std::string _fontFile);

        TextBitmap getTextFromString(std::string str);
    private:
        void renderCharacter(FT_Bitmap bitmap, unsigned int offsetX, unsigned int offsetY);

        void renderCharToBitmap(FT_Bitmap bitmap, unsigned int offsetX, unsigned int offsetY);
        
        void resizeBuffer(unsigned int newStride, unsigned int newRows);

        unsigned int stride = 0;
        unsigned int rows = 0;

        int pen_x = -1;
        int pen_y = -1;

        std::vector<unsigned char> charbitmap;

        std::string fontFile;

        FT_Library library;
        FT_Error error;
        FT_Face face; 
};

#endif
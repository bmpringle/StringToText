#include "StringToText/StringToText.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

int main() {

    StringToTextConverter unifontConverter = StringToTextConverter("unifont-13.0.06.ttf");

    TextBitmap unifontText = unifontConverter.getTextFromString("the quick brown fox jumped over the lazy dog.\nhello, it is i, the muffin man!");

    stbi_write_png("gnu_unifont.png", unifontText.stride, unifontText.rows, 1, unifontText.bitmap.data(), unifontText.stride);

    StringToTextConverter comicSansConverter = StringToTextConverter("comic_sans.ttf");

    TextBitmap comicSansText = comicSansConverter.getTextFromString("the quick brown fox jumped over the lazy dog.\nhello, it is i, the muffin man!");

    stbi_write_png("comic_sans.png", comicSansText.stride, comicSansText.rows, 1, comicSansText.bitmap.data(), comicSansText.stride);
}


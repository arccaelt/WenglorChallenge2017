#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "../bitmap_image.hpp"
#include <string>
#include <windows.h>

struct Rect
{
    int x, y;
    int w, h;
};

class BMPImage
{
public:
    bitmap_image *img = NULL;
    int w, h;

    BMPImage(std::string path);
    std::vector<Rect> getWhiteSpots(int w, int h);
    void copyLogo(BMPImage &logo, Rect &r);

    // defined here so that it will be inlined by default
    void save(std::string path)
    {
        img->save_image(path);
    }

    ~BMPImage();
};

#endif // BMPIMAGE_H

#include "BMPImage.h"

BMPImage::BMPImage(std::string path)
{
    img = new bitmap_image(path);
    if(!img)
    {
        std::cout << "Can't load image: " << path << "\n";
        exit(EXIT_FAILURE);
    }

    w = img->width();
    h = img->height();
}

void BMPImage::copyLogo(BMPImage &logo, Rect &r)
{
    for(int x = r.x, xl = 0; x < r.x + r.w; x++, xl++)
    {
        for(int y = r.y, yl = 0; y < r.y + r.h; y++, yl++)
        {
            rgb_t r = {logo.img->get_pixel(xl, yl)};
            img->set_pixel(x, y, r);
        }
    }
}

std::vector<Rect> BMPImage::getWhiteSpots(int logow, int logoh)
{
    std::vector<Rect> founds;
    for(int x = 0; x < w && (x + logow) <= w; x++)
    {
        for(int y = 0; y < h && (y + logoh) <= h; y++)
        {
            bool good = true;
            //std::cout << "search: " << x << " " << y << " " << x + logow << " " << y + logoh << "\n";
            for(int a = x; a < x + logow; a++)
            {
                for(int b = y; b < y + logoh; b++)
                {
                    rgb_t pixel = img->get_pixel(a, b);
                    unsigned int rv = (unsigned int)pixel.red;
                    unsigned int gv = (unsigned int)pixel.green;
                    unsigned int bv = (unsigned int)pixel.blue;

                    //std::cout << rv << " " << gv << " " << bv << "\n";

                    if(!rv || !gv || !bv)
                    {
                        good = false;
                        goto end;
                    }
                }
            }
            end:
                ;

            if(good)
            {
                //std::cout << "good: " << x << " " << y
                //          << " " << x + logow << " " << y + logoh
                //          << " " << "\n";

                Rect r = {x, y, logow, logoh};
                founds.push_back(r);
            }
        }
    }

    return founds;
}


BMPImage::~BMPImage()
{
    delete img;
}

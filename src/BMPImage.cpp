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
    for(int x = r.y, xl = 0; x < r.y + logo.w; x++, xl++)
    {
        for(int y = r.x, yl = 0; y < r.x + logo.h; y++, yl++)
        {
            rgb_t r = {logo.img->get_pixel(xl, yl)};
            img->set_pixel(x, y, r);
        }
    }
}

// TODO: some logo pixels are not copied.
std::vector<Rect> BMPImage::getWhiteSpots(int &logow, int &logoh)
{
    std::vector<Rect> founds;

    // cache this so we don't have to query it over and over again and
    // waste CPU cycles
    unsigned int bytesPerPixel = img->bytes_per_pixel();

    /*
    * The algorithm is based on the naive one(search for all squares that are
    * white and can contain the logo) just a bit optimized.
    */

    // The first optimization is based on iterating the image only in rects that have
    // the same dimensions as the logo.
    for(int x = logoh - 1; x < h; x += logoh)
    {
        // The currentRow stores a pointer to the memory location where the xth
        // row of pixels of the image is stored.
        unsigned char *currentRow = img->row(x);

        // NOTE: We have to do padding based on bytesPerPixel to get the actual pixels from
        // this row.
        // Example: if bytesPerPixel = 3, then the first three bytes(unsigned char) contains:
        //    first byte - read
        //    second byte - green
        //    third byte - blue
        for(int y = bytesPerPixel; y < w * bytesPerPixel; y += logow * bytesPerPixel)
        {
            // if the currenty pixel is white
            if((int)currentRow[y] == 255)
            {
                bool good = true;

                // This is the fun part. Here, from this pixel that we know is white,
                // we go in all four directions: top, bottom, left, right until we hit
                // a black pixel or we arrive at the end of the image.
                // After that, we will have 4 variables: left_size, right_size, top_size, bot_size
                // that will tell us how many rows/cols in all directions from this pixel we
                // can go and be sure we will have also white pixels.
                // These variables allows us to create a "BOUNDING BOX". In simpler terms,
                // after we have these variables we can check if the square/rect they bound
                // has the logo dimension and if it's true then we can also check if the containing
                // pixels of the rect(until now we have only the edges that delimits our rect) are also whote

                // NOTE: padding occurs here as well

                // go left
                int left_size = y - bytesPerPixel;
                while(left_size >= 0 && (int)currentRow[left_size] == 255)
                {
                    left_size -= bytesPerPixel;
                }

                // go right
                int right_size = y + bytesPerPixel;
                while(right_size < w * bytesPerPixel && (int)currentRow[right_size] == 255)
                {
                    right_size += bytesPerPixel;
                }

                // if by going left and right we have a row with the size equal to the
                // logo's width
                if(right_size - left_size - bytesPerPixel == logow * bytesPerPixel)
                {
                    // go top
                    int top_size = x - 1;
                    while(top_size >= 0 && (int)(img->row(top_size)[y] == 255))
                    {
                        top_size--;
                    }

                    // go bot
                    int bot_size = x + 1;
                    while(bot_size < h && (int)(img->row(bot_size)[y]) == 255)
                    {
                        bot_size++;
                    }

                    // if by going top and bot we have a row with the size equal to the
                    // logo's width
                    if(bot_size - top_size - 1 == logoh)
                    {
                        // check all pixels
                        // NOTE: As i've said this is still the good ol' naive algorithm
                        // in essence but with the checks and loops from above we managed
                        // to reduce the amount of time we are forced to get at this for loop
                        for(int cr = top_size + 1; cr < bot_size; cr++)
                        {
                            currentRow = img->row(cr);
                            for(int cc = left_size + bytesPerPixel; cc < right_size; cc += bytesPerPixel)
                            {
                                // it at least one is black then end the search
                                if((int)currentRow[cc] == 0)
                                {
                                    good = false;
                                    goto end;
                                }
                            }
                        }
                    }
                    else
                    {
                        good = false;
                    }

                    end:
                    ;

                    if(good)
                    {
                        //std::cout << left_size << "\n";
                        Rect r = {top_size < 0 ? 0 : top_size, (left_size > 0 ? left_size : 0) / img->bytes_per_pixel(), logow, logoh};
                        founds.push_back(r);
                    }
                }
            }
        }
    }

    return founds;
}


BMPImage::~BMPImage()
{
    delete img;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

Image convolve_image(const Image& im, const Image& filter, bool preserve)
{

    assert(filter.c==1);
    Image conv(im.w, im.h, im.c);

    for (int c = 0; c < im.c; c ++) {
        for (int y = 0; y < im.h; y ++) {
            for (int x = 0; x < im.w; x ++) {

                // The starting coordinates in image
                int sx = x - (filter.w / 2);
                int sy = y - (filter.h / 2);

                float sum = 0.0;

                for (int fy = 0; fy < filter.h; fy ++) {
                    for (int fx = 0; fx < filter.w; fx ++) {
                        sum += filter(fx, fy, 0) * im.clamped_pixel(sx + fx, sy + fy, c);
                    }
                }

                conv(x,y,c) = sum;
            }
        }
    }


    if (!preserve) {
        for (int y = 0; y < conv.h; y ++) {
            for (int x = 0; x < conv.w; x ++) {
                float sum = 0;
                for (int c = 0; c < conv.c; c ++) {
                    sum += conv(x,y,c);
                }
                conv(x,y,0) = sum;
            }
        }
        conv.c = 1;
    }


    return conv;
}

Image make_gx_filter()
{

    Image f (3,3,1);

    f(0,0,0) = -1;
    f(1,0,0) = 0;
    f(2,0,0) = 1;
    f(0,1,0) = -2;
    f(1,1,0) = 0;
    f(2,1,0) = 2;
    f(0,2,0) = -1;
    f(1,2,0) = 0;
    f(2,2,0) = 1;

    return f;
}

Image make_gy_filter()
{

    Image f (3,3,1);

    f(0,0,0) = -1;
    f(1,0,0) = -2;
    f(2,0,0) = -1;
    f(0,1,0) = 0;
    f(1,1,0) = 0;
    f(2,1,0) = 0;
    f(0,2,0) = 1;
    f(1,2,0) = 2;
    f(2,2,0) = 1;

    return f;
}

void feature_normalize(Image& im)
{
    assert(im.w*im.h);



    for (int c = 0; c < im.c; c ++) {

        float min_val = im(0,0,c);
        float max_val = im(0,0,c);

        for (int y = 0; y < im.h; y ++) {
            for (int x = 0; x < im.w; x ++) {
                min_val = min (min_val, im(x,y,c));
                max_val = max (max_val, im(x,y,c));
            }
        }

        float range = max_val - min_val;

        for (int y = 0; y < im.h; y ++) {
            for (int x = 0; x < im.w; x++) {
                if (range) {
                    im(x,y,c) = (im(x,y,c) - min_val) / range;
                }
                else
                    im(x,y,c) = 0;
            }
        }
    }
}


Image quant_and_rec (Image& im1, Image& im2) {
    Image ret(im1.w,im1.h,im1.c);
        for (int y = 0; y < im1.h; y++) {
            for (int x = 0; x < im1.w; x++) {

                //edge
                int a = im2(x,y,0);

                //bitwise not edge
                if (a == 0){ a = (unsigned int) 255;}
                else {a = (unsigned int) 0;}

                //color quantization [256/24]^3

                int b = (unsigned int) floor(im1(x,y,0) * 255 /24) * 24;
                int c = (unsigned int) floor(im1(x,y,1) * 255 / 24) * 24;;
                int d = (unsigned int) floor(im1(x,y,2) * 255 / 24) * 24;

                //bitwise and
                ret(x,y,0 ) = (a&b) / 255.0;
                ret(x,y,1) = (a&c) / 255.0;
                ret(x,y,2 ) = (a&d) / 255.0;

            }
        }
    return ret;
}

pair<Image,Image> sobel_image(const Image& im)
{

    Image fx = make_gx_filter();
    Image fy = make_gy_filter();

    Image Gx = convolve_image (im, fx, false);
    Image Gy = convolve_image (im, fy, false);


    Image G(im.w, im.h, 1);
    Image T(im.w, im.h, 1);


    for (int y = 0; y < im.h; y ++) {
        for (int x = 0; x < im.w; x ++) {
            G(x,y,0) = sqrtf ( pow(Gx(x,y,0), 2) + pow(Gy(x,y,0), 2));
            T(x,y,0) = atan2f( Gy(x,y,0) , Gx(x,y,0));
        }
    }


    return {G,T};
}











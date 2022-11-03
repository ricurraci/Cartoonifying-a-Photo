#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846

//1)Bilateral filter

//downsampled by a factor of 4
Image resizing(Image& im){
    Image f = make_box_filter(7);
    Image blur = convolve_image(im,f,true);
    Image rsz = nearest_resize(blur,im.w/4,im.h/4);
    return rsz;
}

//apply bilateral filter

//2) Median Filter

Image median_filter_color(const Image& im,float sigma){ //filtro 7x7 sigma è uguale a 1
    Image new_im = im;
    Image f = make_gaussian_filter(sigma);
    Image conv = convolve_image(new_im, f, true);

    return conv;
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

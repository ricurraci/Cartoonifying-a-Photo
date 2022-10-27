#include "image.h"

Image dilation(const Image& im, const int n) {

    Image dilate(im.w, im.h, im.c);

    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {

                int sx = x - (n / 2);
                int sy = y - (n / 2);

                for (int fy = 0; fy < n; fy++) {
                    for (int fx = 0; fx < n; fx++) {
                        if (im.clamped_pixel(sx + fx, sy + fy, c) == 1) {
                            dilate(x, y, c) = 1;
                            break;
                        }
                    }
                    if (dilate(x,y,c)==1){
                        break;
                    }
                }
            }
        }
    }
    return dilate;
}
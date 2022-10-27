#include "image.h"


float Image::pixel_nearest(float x, float y, int c) const {
    float pixel = clamped_pixel((int) round(x), (int) round(y), c);
    return pixel;
}

Image nearest_resize(const Image &im, int w, int h) {
    Image ret(w,h,im.c);

    float scalingw = (float)im.w / (float) w;
    float bw = 0.5 * (scalingw-1);
    float scalingh = (float)im.h / (float) h;
    float bh = 0.5 * (scalingh-1);
    float a,b;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {

            a = x * scalingw + bw;
            b = y * scalingh + bh;
            for (int c = 0; c < im.c; c++) {
                ret(x, y, c) = im.pixel_nearest(a, b, c);
            }
        }
    }
    return ret;
}






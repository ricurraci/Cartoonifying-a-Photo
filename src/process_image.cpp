
#include <cassert>
#include "image.h"

using namespace std;

Image rgb_to_grayscale(const Image &im) {
    assert(im.c == 3);
    Image gray(im.w,im.h,1);

    for (int i = 0; i < im.h; i++){
        for (int j = 0; j < im.w; j++){
            float r = im(j,i,0);
            float g = im(j,i,1);
            float b = im(j,i,2);
            float media = 0.299 * r + 0.587 * g + 0.114 * b;
            gray( j, i, 0) = media;
        }
    }
    return gray;
}















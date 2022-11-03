#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846
using namespace std;

// 1) median filter
//  per ridurre il salt and pepper noise, si usa un filtro kernel 7x7 (centroid = baricentro)
//  i pixel RGB al baricentro sono settati per essere la media dei 49 RGB dei pixel vicini.
Image smooth_image(const Image& im,float sigma){
    Image new_im = rgb_to_grayscale(im);
    Image f = make_gaussian_filter(sigma);
    Image conv = convolve_image(new_im, f, false);
    return conv;
}



// 2) edge detection
//usa Canny edge detection
pair<Image,Image> compute_gradient(const Image& im, float sigma){

    Image median = smooth_image(im, sigma);
    pair<Image, Image> couple = sobel_image(median);

    Image mag = couple.first; //magnitude
    Image dir = couple.second;//direction

    feature_normalize(mag);
    feature_normalize(dir);

    return{mag, dir};
}

Image non_maximum_supp(const Image& G, const Image& theta){
    Image Z(G.w, G.h, 1);
    Image angle(G.w, G.h, 1);

    // Round the gradient direction to the nearest 45 degrees
    for (int y = 0; y < G.h; y ++) {
        for (int x = 0; x < G.w; x ++) {
            angle(x,y,0) = (theta(x,y,0) * 180.0 ) / M_PI;
            if (angle(x,y,0) < 0)
                angle(x,y,0) += 180;
        }
    }


    for (int y = 1; y < G.h-1; y ++) {
        for (int x = 1; x < G.w-1; x ++) {
            float q = 255.0;
            float r = 255.0;

            // angle 0
            if (((0 <= angle(x,y,0) && angle(x,y,0)) < 22.5) || (157.5 <= angle(x,y,0) && angle(x,y,0) <= 180)) {
                q = G(x+1, y, 0);
                r = G(x-1, y, 0);
            }

            // angle 45
            if ((22.5 <= angle(x,y,0) && angle(x,y,0) < 67.5)) {
                q = G(x - 1, y + 1, 0);
                r = G(x + 1, y - 1, 0);
            }
                // angle 90
            else if ((67.5 <= angle(x,y,0) &&  angle(x,y,0) < 112.5)){
                q = G(x, y+1, 0);
                r = G(x, y-1, 0);
            }

                // angle 135
            else if ((112.5 <= angle(x,y,0) && angle(x,y,0) < 157.5)) {
                q = G(x - 1, y - 1, 0);
                r = G(x + 1, y + 1, 0);
            }

            if ((G(x, y, 0) >= q) && (G(x, y, 0) >= r))
                Z(x, y, 0) = G(x,y,0);
            else
                Z(x,y,0) = 0;
        }
    }
    return Z;

}

Image double_thresholding(const Image& im, float lowThresholdRatio, float highThresholdRatio, float strong, float weak){

    Image res(im.w, im.h, im.c);

    for (int y = 0; y < im.h; y ++) {
        for (int x = 0; x < im.w; x ++) {
            // strong
            if(im(x,y,0) >= highThresholdRatio)
                res(x,y,0) = strong;
                // weak
            else if (im(x,y,0) >= lowThresholdRatio && im(x,y,0) < highThresholdRatio)
                res(x,y,0) = weak;
                // zeros
            else
                res(x,y,0) = 0.0;
        }
    }
    return res;
}

Image edge_tracking(const Image& im, float weak, float strong){

    Image res(im.w, im.h, im.c);
    for (int y = 1; y < im.h-1; y ++) {
        for (int x = 1; x < im.w - 1; x++) {
            res(x,y,0) = im(x,y,0);
        }
    }

    for (int y = 1; y < im.h-1; y ++) {
        for (int x = 1; x < im.w - 1; x++) {
            if(im(x,y,0) == weak){
                if((im(x-1,y+1,0) == strong) or (im(x,y+1,0) == strong) or (im(x+1,y+1,0) == strong) or
                   (im(x-1,y,0) == strong) or (im(x+1,y,0) == strong) or (im(x-1,y-1,0) == strong) or
                   (im(x,y-1,0) == strong) or (im(x+1,y-1,0) == strong))
                    res(x,y,0) = 1;
                else
                    res(x,y,0) = 0;
            }
        }
    }
    return res;

}
// 3) morphological operations

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

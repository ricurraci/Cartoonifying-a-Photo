#include "image.h"

pair<Image,Image> compute_gradient(const Image& im, float sigma)
{
    pair<Image,Image> gradient = sobel_image(im);
    Image mag = gradient.first;
    Image dir = gradient.second;
    feature_normalize(mag);
    feature_normalize(dir);

    return{mag, dir};
}

Image non_maximum_supp(const Image& G, const Image& theta)
{
    Image ret(G.w, G.h, 1);
    Image angle(G.w, G.h, 1);

    for (int y = 0; y<angle.h; y++) {
        for (int x = 0; x < angle.w; x++) {
            angle(x,y) = (theta(x,y) * 180.0) / M_PI;
            if (angle(x,y) < 0) {
                angle(x, y) += 180;
            }
        }
    }

    for (int y = 1; y<G.h-1; y++){
        for (int x = 1; x<G.w-1; x++){
            float q = 255.0;
            float r = 255.0;
            float angolo = angle(x,y);

            //angolo 0
            if ( (angolo >= 0 && angolo<22.5) || (angolo>=157.5 && angolo <= 180)) {
                q = G(x,y+1);
                r = G(x,y-1);
            }


                //angolo 45
            else if ( (angolo>= 22.5 && angolo<67.5)) {
                q = G(x+1,y-1);
                r = G(x-1,y+1);
            }

                //angolo 90
            else if ( (angolo>= 67.5 && angolo<112.5)) {
                q = G(x+1,y);
                r = G(x-1,y);
            }

                //angolo 135
            else if ( (angolo>= 112.5 && angolo<157.5)) {
                q = G(x-1,y-1);
                r = G(x+1,y+1);
            }

            if ( (G(x,y) >= q) && (G(x,y) >= r) ){
                ret(x,y) = G(x,y);
            }
            else ret(x,y) = 0.0;
        }
    }
    return ret;

}


Image double_thresholding(const Image& im, float lowThresholdRatio, float highThresholdRatio, float strong, float weak)
{
    Image ret(im.w, im.h, im.c);
    for (int y=0; y<im.h; y++){
        for (int x=0; x<im.w; x++){
            if (im(x,y)>=highThresholdRatio){
                ret(x,y) = strong;
            }
            else if (im(x,y) >= lowThresholdRatio && im(x,y) < highThresholdRatio){
                ret(x,y) = weak;
            }
            else
                ret(x,y) = 0.0;
        }
    }
    return ret;
}


Image edge_tracking(const Image& im, float weak, float strong)
{
    Image ret (im.w, im.h, 1);
    for (int y = 1; y < im.h-1; y ++) {
        for (int x = 1; x < im.w - 1; x++) {
            ret(x,y) = im(x,y);
        }
    }
    for (int y = 1; y<im.h-1; y++){
        for (int x = 1; x<im.w; x++){
            if (im(x,y)==weak){
                if((im(x+1,y-1,0) == strong) or (im(x+1,y,0) == strong) or (im(x+1,y+1,0) == strong) or
                   (im(x,y-1,0) == strong) or (im(x,y+1,0) == strong) or (im(x-1,y-1,0) == strong) or
                   (im(x-1,y,0) == strong) or (im(x-1,y+1,0) == strong))
                    ret(x,y,0) = strong;
                else
                    ret(x,y,0) = 0;
            }
        }
    }
    return ret;

}

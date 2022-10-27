#include <cmath>
#include "image.h"

float gaussian(float n, double sigma) {
    return exp( -(pow(n, 2))/(2 * pow(sigma, 2))) / (2 * M_PI * pow(sigma, 2));

}


void update_hist_(vector<int>& hist, int i, int j, int c, int filter_size, const Image& im){
    for (int k = -filter_size / 2; k <= filter_size / 2 ; k++) {
        hist[round(im.clamped_pixel(i - filter_size / 2 -1, j+k , c) * 255)]--;
        hist[round(im.clamped_pixel(i + filter_size /2, j +k, c) * 255)]++;

    }

}

Image bilateral_filter(Image&im, double sigma1, double sigma2){
    int filter_size = sigma2;
    Image bilateral (im.w, im.h, im.c);

    for (int c=0; c<im.c; c++) {

        //Histogram initialization (pixel 0,0)

        vector<int> histogram(256, 0);
        float val = 0;
        float weight = 0;

        int sx = -(filter_size / 2);
        int sy = -(filter_size / 2);
        for (int fy = 0; fy < filter_size; fy++) {
            for (int fx = 0; fx < filter_size; fx++) {
                histogram[round(im.clamped_pixel(sx + fx, sy + fy, c) * 255)]++;
            }
        }

        for (int counter = 0; counter < 256 ; counter++) {
            if (histogram[counter]!=0){
                float g1 = gaussian(im.clamped_pixel(0, 0,c) * 255 - counter, sigma1);
                val += histogram[counter] * counter * g1;
                weight += histogram[counter] * g1;
            }
        }
        bilateral(0, 0, c) = (val/weight) / 255.0;



        for (int i = 0; i < im.w; i++) {

            if (i!=0){
                update_hist_(histogram, i, 0, c, filter_size, im);

                val = 0;
                weight = 0;

                for (int counter = 0; counter < 256 ; counter++) {
                    if (histogram[counter]!=0){
                        float g1 = gaussian( im.clamped_pixel(i, 0,c) * 255 - counter, sigma1);
                        val += histogram[counter] * counter * g1;
                        weight += histogram [counter] * g1;
                    }
                }
                bilateral(i, 0, c) = (val/weight) / 255.0;

            }

            //Scan down

            for (int j = 1 ; j < im.h; j++) {
                for (int k = -filter_size / 2; k <= filter_size / 2; k++) {
                    histogram[round(im.clamped_pixel(i + k, j - (filter_size / 2) - 1, c) * 255)]--;
                    histogram[round(im.clamped_pixel(i + k, j + filter_size / 2, c) * 255)]++;

                }

                val = 0;
                weight = 0;

                for (int counter = 0; counter < 256 ; counter++) {
                    if (histogram[counter]!=0){
                        float g1 = gaussian(im.clamped_pixel(i, j,c) * 255 - counter, sigma1);
                        val += histogram[counter] * counter * g1;
                        weight += histogram[counter] * g1;
                    }
                }
                bilateral(i, j, c) = (val/weight) / 255.0;

            }

            if (i!= im.w-1) {
                i++;
                update_hist_(histogram, i, im.h-1, c, filter_size, im);

                val = 0;
                weight = 0;
                for (int counter = 0; counter < 256 ; counter++) {
                    if (histogram[counter]!=0){
                        float g1 = gaussian(im.clamped_pixel(i, im.h-1,c) * 255 - counter, sigma1);
                        val += histogram[counter] * counter * g1;
                        weight += histogram[counter] * g1;
                    }
                }
                bilateral(i, im.h-1 , c) = (val/weight) / 255.0;


                //Scan up

                for (int j = im.h - 2; j >= 0; j--) {
                    for (int k = -filter_size / 2; k <= filter_size / 2; k++) {
                        histogram[round(im.clamped_pixel(i + k, j - filter_size / 2 , c) * 255)]++;
                        histogram[round(im.clamped_pixel(i + k, j + filter_size / 2 +1, c) * 255)]--;

                    }

                    val = 0;
                    weight = 0;
                    for (int counter = 0; counter < 256 ; counter++) {
                        if (histogram[counter]!=0){
                            float g1 = gaussian(im.clamped_pixel(i, j,c) * 255 - counter, sigma1);
                            //if (g1 == 0) { cout << (im.clamped_pixel(i, j,c) * 255 -counter) << "\n";}
                            val += histogram[counter] * counter * g1;
                            weight += histogram[counter] * g1;
                        }
                    }
                    bilateral(i, j, c) = (val/weight) / 255.0;


                }
            }
        }

    }
    return bilateral;
}

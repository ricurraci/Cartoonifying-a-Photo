
#include "image.h"

void update_hist(vector<int>& hist, int i, int j, int c, int filter_size, const Image& im){
    for (int k = -filter_size / 2; k <= filter_size / 2 ; k++) {
        hist[round(im.clamped_pixel(i - filter_size / 2 -1, j+k , c) * 255)]--;
        hist[round(im.clamped_pixel(i + filter_size /2, j +k, c) * 255)]++;

    }

}

Image median_filter(const Image&im){
    int filter_size = 7;
    Image median (im.w, im.h, im.c);

    for (int c=0; c<im.c; c++) {

        //Histogram initialization (pixel 0,0)

        vector<int> histogram(256, 0);
        int sx = -(filter_size / 2);
        int sy = -(filter_size / 2);
        for (int fy = 0; fy < filter_size; fy++) {
            for (int fx = 0; fx < filter_size; fx++) {
                histogram[round(im.clamped_pixel(sx + fx, sy + fy, c) * 255)]++;
            }
        }
        int sum = 0;
        int counter;
        for (counter = 0; sum <= 49 / 2; counter++) {
            sum += histogram[counter];
        }
        int med = counter;
        median(0, 0, c) = (med-1) / 255.0;



        for (int i = 0; i < im.w; i++) {

            if (i!=0){
                update_hist(histogram, i, 0, c, filter_size, im);
                sum = 0;
                for (counter = 0; sum < 49 / 2; counter++) {
                    sum += histogram[counter];
                }

                med = counter;
                median(i, 0, c) = (med-1) / 255.0;
            }

            //Scan down

            for (int j = 1 ; j < im.h; j++) {
                for (int k = -filter_size / 2; k <= filter_size / 2; k++) {
                    histogram[round(im.clamped_pixel(i + k, j - (filter_size / 2) - 1, c) * 255)]--;
                    histogram[round(im.clamped_pixel(i + k, j + filter_size / 2, c) * 255)]++;

                }
                sum = 0;
                for (counter = 0; sum <= 49 / 2; counter++) {
                    sum += histogram[counter];
                }


                med = counter;
                median(i, j, c) = (med-1) / 255.0;

            }

            if (i!= im.w-1) {
                i++;
                update_hist(histogram, i, im.h-1, c, filter_size, im);
                sum = 0;
                for (counter = 0; sum < 49 / 2; counter++) {
                    sum += histogram[counter];
                }

                med = counter;
                median(i, im.h-1, c) = (med-1) / 255.0;

                //Scan up

                for (int j = im.h - 2; j >= 0; j--) {
                    for (int k = -filter_size / 2; k <= filter_size / 2; k++) {
                        histogram[round(im.clamped_pixel(i + k, j - filter_size / 2 , c) * 255)]++;
                        histogram[round(im.clamped_pixel(i + k, j + filter_size / 2 +1, c) * 255)]--;

                    }
                    sum = 0;
                    for (counter = 0; sum < 49 / 2; counter++) {
                        sum += histogram[counter];
                    }

                    med = counter;
                    median(i, j, c) = (med-1) / 255.0;

                }
            }
        }

    }
    return median;
}

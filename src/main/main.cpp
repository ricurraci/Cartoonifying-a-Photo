#include "../image.h"
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
    
    
	cout << "Programma per cartonizzare le immagini \n" ;
    cout << "Inserire il path dell'immagine da elaborare:" ;
    
    char url[200];
    cin >> url;

    Image im = load_image(url);
    Image final_image;
    
    clock_t begin = clock();

    Image gray = rgb_to_grayscale(im);

    //EDGES

    //1 median filter
    Image median_filtered = median_filter(gray);

    //2 canny edge detection
    pair<Image,Image> grad = compute_gradient(median_filtered, 1.2);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.1, 0.15, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong);

    //3 dilation

    Image edge_track_dilated = dilation(edge_track, 2);

    //COLORS

    // 1 Resize and bilateral
    Image resized = nearest_resize(im, im.w/4, im.h/4);
    Image bilateral = bilateral_filter(resized, 3.0, 3.0);
    bilateral = nearest_resize(bilateral, bilateral.w*4, bilateral.h*4);


    bilateral = median_filter(bilateral);


    // Color quantization + recombination
    Image toonified = quant_and_rec (bilateral, edge_track_dilated);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    cout << "Runtime: " << elapsed_secs <<"\n";
       
    char url_out[200];
    strcpy(url_out, url);
    strcat(url_out, "_cartoonized");
    toonified.save_png(url_out);

}

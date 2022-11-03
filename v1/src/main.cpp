#include "image.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace std;

Image process(Image& im){
    //edges
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.03, 0.17, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong); //edge_track è  il bordo dell'immagine
    Image edge_track_dilated = dilation(edge_track, 2);

    //colors
    Image im_sz = nearest_resize(im, im.w/4, im.h/4);
    Image bf;
    bf = bilateral_filter(im_sz, 3, 0.1);
    
    Image im_bigger = bilinear_resize(bf, bf.w *4 , bf.h *4);
    Image mfc= median_filter_color(im_bigger,1);

    //recombine
    Image merge = quant_and_rec( mfc, edge_track_dilated );
    return merge;
}



int main() {
    cout << "Programma per cartonizzare immagini \n" ;
    cout << "Inserire il path dell'immagine da elaborare:" ;

    char url[200];
    
    clock_t begin = clock();

    cin >> url;
    Image start_image = load_image(url);
    char *dot = strrchr( url, '.');
    if(dot != NULL && dot != url)
        *dot = '\0';


    int h=start_image.h;
    int w=start_image.w;
    cout << "L'immagine ha dimensione (h x w) " << h << " x "<< w << "\n";

    Image final_image;
    
    final_image = process(start_image);
    

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    cout << "Runtime: " << elapsed_secs <<"\n";
    
    char url_out[200];
    strcpy(url_out, url);
    strcat(url_out, "_cartoonized");
    final_image.save_png(url_out);
    cout << "L'immagine elaborata è stata salvata come " << url_out<<"\n";


    return 0;
}


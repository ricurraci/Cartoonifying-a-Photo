#pragma once

#include <cassert>
#include <cstring>
#include <cmath>

#include <algorithm>
#include <string>
#include <vector>
#include <stdexcept>
#include "iostream"

using namespace std;


struct Image
  {
  int w=0;
  int h=0;
  int c=0;
  float* data=nullptr;
  
  // constructor
  Image() = default;
  
  Image(int w, int h, int c=1) : w(w), h(h), c(c), data(nullptr) 
    {
    assert(c>=0 && w>=0 && h>=0 && "Invalid image sizes");
    
    if(w*h*c)
      data=(float*)calloc(w*h*c,sizeof(float));
    
    }
  
  // destructor
  ~Image() { free(data); }
  
  // copy constructor
  Image(const Image& from) : data(nullptr) { *this=from; }
  
  // move constructor
  Image(Image&& from)  : data(nullptr) { *this=move(from); }
  
  // copy assignment
  Image& operator=(const Image& from)
    {
    if(this==&from)return *this;
    
    if(data){free(data);data=nullptr;}
    w=h=c=0;
    // allocating data for the new image
    data=(float*)calloc(from.w*from.h*from.c,sizeof(float));
    
    w=from.w;
    h=from.h;
    c=from.c;
    memcpy(data,from.data,sizeof(float)*w*h*c);
    return *this;
    }
  
  // move assignment
  Image& operator=(Image&& from)
    {
    if(this==&from)return *this;
    
    if(data)free(data);
    
    w=from.w;
    h=from.h;
    c=from.c;
    data=from.data;
    
    from.data=nullptr;
    from.w=from.h=from.c=0;
    
    return *this;
    }
  
  // pixel access
  
  int pixel_address(int x, int y, int ch) const
    {
    return ch*w*h + y*w + x;
    }
  
  float& operator()(int x, int y, int ch)
    {
        if (!(ch<c && ch>=0 && x<w && x>=0 && y<h && y>=0)){
            std::cout << "access out of bounds" << std::endl;
        }
    assert(ch<c && ch>=0 && x<w && x>=0 && y<h && y>=0 && "access out of bounds");
    return data[pixel_address(x,y,ch)];
    }
  
  float& operator()(int x, int y)
    {
    assert(c==1 && x<w && x>=0 && y<h && y>=0 && "access out of bounds");
    return data[pixel_address(x,y,0)];
    }
  
  const float& operator()(int x, int y, int ch) const 
    {
    //printf("%d %d %d   %d %d %d\n",w,h,c,x,y,ch);
    assert(ch<c && ch>=0 && x<w && x>=0 && y<h && y>=0 && "access out of bounds");
    return data[pixel_address(x,y,ch)];
    }
  
  const float& operator()(int x, int y) const 
    {
    assert(c==1 && x<w && x>=0 && y<h && y>=0 && "access out of bounds");
    return data[pixel_address(x,y,0)];
    }
  
        float& pixel(int x, int y, int ch)       { return operator()(x,y,ch); }
  const float& pixel(int x, int y, int ch) const { return operator()(x,y,ch); }
        float& pixel(int x, int y)               { return operator()(x,y); }
  const float& pixel(int x, int y) const         { return operator()(x,y); }
  
  
  float clamped_pixel(int x, int y, int ch) const
    {
    assert(ch<c && ch>=0);
    if(x>=w)x=w-1;
    if(y>=h)y=h-1;
    if(x<0)x=0;
    if(y<0)y=0;
    return pixel(x,y,ch);
    }


  
  // member functions for inexact access
  
  float pixel_nearest (float x, float y, int c) const;

  void shift(int c, float v);

  
  // Image I/O member functions
  void load_image  (const string& filename);
  void save_png    (const string& filename) const;
  
  };

// Image I/O functions
inline Image load_image  (const string& filename) { Image im; im.load_image(filename);  return im; }


// Basic operations
Image rgb_to_grayscale(const Image& im);

// Resizing
Image nearest_resize (const Image& im, int w, int h);



// Filtering
Image convolve_image(const Image& im, const Image& filter, bool preserve);
Image make_gx_filter(void);
Image make_gy_filter(void);
void feature_normalize(Image& im);
Image quant_and_rec(Image& im1, Image& im2);
Image bilateral_filter(Image& source, double sigma1, double sigma2);


pair<Image,Image> sobel_image(const Image&  im);

// Edging
Image median_filter(const Image&im);
pair<Image,Image> compute_gradient(const Image& im, float sigma);
Image non_maximum_supp(const Image& G, const Image& theta);
Image double_thresholding(const Image& im, float lowThreshold, float highThreshold, float strong, float weak);
Image edge_tracking(const Image& im, float weak, float strong);
Image dilation(const Image& im, const int n);



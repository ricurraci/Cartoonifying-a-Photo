cmake_minimum_required(VERSION 3.16)
project(cartoonify)

set(CMAKE_CXX_STANDARD 14)

add_executable(cartoonify main.cpp image.h load_image.cpp stb_image.h filter_image.cpp stb_image_write.h process_image.cpp resize_image.cpp median_filter.cpp canny.cpp dilation.cpp bilateral_filter.cpp)

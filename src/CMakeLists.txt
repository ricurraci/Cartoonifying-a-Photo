cmake_minimum_required(VERSION 3.16)
project(cartoonifying_photo)

set(CMAKE_CXX_STANDARD 14)

add_executable(cartoonifying_photo main.cpp image.h load_image.cpp stb_image.h stb_image_write.h utils.h utils.cpp process_image.cpp resize_image.cpp filter_image.cpp edges.cpp colors.cpp)

cmake_minimum_required(VERSION 3.8)

project(Toonify)

set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -std=c++11 -pthread -O3 -g -fPIC")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/)

add_library(uwimg++ SHARED
	    image.h
            load_image.cpp
            stb_image.h
	    filter_image.cpp
            stb_image_write.h
            process_image.cpp
	    resize_image.cpp

            median_filter.cpp
	    canny.cpp
	    dilation.cpp
            bilateral_filter.cpp
        )

link_libraries(uwimg++ m stdc++)

add_executable(cartoonify main/main.cpp)




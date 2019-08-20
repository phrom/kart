cmake_minimum_required(VERSION 3.1)

project(kart)

find_package(SDL2 REQUIRED)

add_executable(kart
        src/curve.c
        src/data.c
        src/main.c
        src/path.c
        src/point.c
        src/runner.c
        src/sdl.c
        src/utils.c
        src/vector.c)

target_include_directories(kart PRIVATE include ${SDL2_INCLUDE_DIRS})

target_link_libraries(kart ${SDL2_LIBRARIES} SDL2_image SDL2_ttf)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/sanmarino12counter.png
        ${CMAKE_CURRENT_BINARY_DIR}/sanmarino12counter.png
        COPYONLY)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/DejaVuSans.ttf
        ${CMAKE_CURRENT_BINARY_DIR}/DejaVuSans.ttf
        COPYONLY)
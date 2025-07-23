cmake_minimum_required(VERSION 3.14)
project(app)

add_subdirectory(lib/SDL2)

add_executable(app src/main.c src/render.c src/raster.c src/scene.c src/object.c src/vector.c src/viewport.c src/image.c)

target_include_directories(app PRIVATE lib/SDL2/include)
target_link_libraries(app PRIVATE SDL2main SDL2-static)
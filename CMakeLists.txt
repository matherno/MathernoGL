cmake_minimum_required(VERSION 3.0)
project(MathernoGL)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include ./dependencies/include)

file(GLOB_RECURSE SOURCES  "src/*.cpp" "include/*.h")
add_library(MathernoGL STATIC ${SOURCES} src/maths/Transform.cpp include/mathernogl/maths/Transform.h include/mathernogl/utils/MappedList.h include/mathernogl/utils/Misc.h src/utils/FontImporter.cpp include/mathernogl/utils/FontImporter.h src/rendering/ScreenTextureRenderer.cpp include/mathernogl/rendering/ScreenTextureRenderer.h)
target_link_libraries(MathernoGL GL GLEW glfw freeimage)

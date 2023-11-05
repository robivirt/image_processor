add_executable(
    image_processor
    image_processor.cpp
    image.cpp
    filters/crop.cpp
    filters/grayscale.cpp
    filters/negative.cpp
    filters/sharp.cpp
    filters/edge.cpp
    filters/blur.cpp
    filters/crystalize.cpp
)

target_include_directories(
    image_processor PUBLIC .
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
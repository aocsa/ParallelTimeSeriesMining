cmake_minimum_required(VERSION 2.8)
add_definitions("-std=c++14")

project(TSDiscovery)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(src)
add_subdirectory(src)

add_executable(motifs motifs.cpp)

add_executable(outliers outliers.cpp)

target_link_libraries(motifs ${CONAN_LIBS})

target_link_libraries(outliers ${CONAN_LIBS})

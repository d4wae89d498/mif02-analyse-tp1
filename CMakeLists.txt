cmake_minimum_required(VERSION 3.10)
project(MonProjet)

find_package(OpenCV REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/base.cpp
    ${CMAKE_SOURCE_DIR}/src/filter.cpp
)

add_executable(MonProjet ${SOURCES})

target_link_libraries(MonProjet ${OpenCV_LIBS})

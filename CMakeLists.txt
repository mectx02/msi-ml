cmake_minimum_required(VERSION 3.12)

add_subdirectory(hidapi)

project(msi-mysticlight)

add_executable(msi-ml main.c)

set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "-g")

target_link_libraries(msi-ml PRIVATE hidapi::hidapi)
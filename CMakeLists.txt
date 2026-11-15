cmake_minimum_required(VERSION 3.16)
project(CTDL_26_27_HCMUE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME}
        main.cpp
        ${PROJECT_SOURCES}
        src/service/BookingService.cpp
        include/ui/Format.h
        src/ui/Console.cpp
        src/ui/Menu.cpp
        src/ui/App.cpp
        include/storage/Directory.h
        src/storage/Directory.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include")

target_compile_definitions(${PROJECT_NAME} PRIVATE
        DATA_DIR="${PROJECT_SOURCE_DIR}/data/")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()


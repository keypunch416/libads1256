cmake_minimum_required(VERSION 2.8.11)
project(ads1256)

add_subdirectory(test)

add_definitions(-Wall)

include_directories(include)

add_definitions(-DADS1256_DEBUG)
add_definitions(-DDAC8532_DEBUG)
add_definitions(-DDEV_DEBUG)

set(SRC
    src/ADS1256.c
    src/DAC8532.c
    src/DEV_Config.c)

add_library(${PROJECT_NAME} STATIC ${SRC})
target_link_libraries(${PROJECT_NAME} bcm2835)
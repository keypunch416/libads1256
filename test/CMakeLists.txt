cmake_minimum_required(VERSION 2.8.11)
project(ads1256-test)

add_definitions(-Wall)

include_directories(../include)

set(SRC
    test.c)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ads1256)
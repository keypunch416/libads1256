cmake_minimum_required(VERSION 2.8.11)

add_definitions(-Wall)

include_directories(../include)

add_executable(test_adc test_adc.c)
target_link_libraries(test_adc ads1256)

add_executable(test_dac test_dac.c)
target_link_libraries(test_dac ads1256)
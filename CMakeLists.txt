cmake_minimum_required(VERSION 3.15)
project(HW1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic-errors -Werror")
link_directories(.)
add_executable(HW1 main.c matamzom.c order.c order.h list.h set.h matamazom.h test_utilities.h)
target_link_libraries(HW1 libmtm.a)
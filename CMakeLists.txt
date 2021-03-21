cmake_minimum_required(VERSION 3.16)
project(ring_buffer CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tests)

add_library(ring_buffer_lib ringbuffer/ringbuffer.hpp)
target_include_directories(ring_buffer_lib PUBLIC ringbuffer)
set_target_properties(ring_buffer_lib PROPERTIES LINKER_LANGUAGE CXX)
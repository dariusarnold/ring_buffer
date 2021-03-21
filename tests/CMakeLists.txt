find_package(Catch2 REQUIRED)

add_executable(ring_buffer_tests catch_test_main.cpp)
target_link_libraries(ring_buffer_tests PRIVATE
        Catch2::Catch2WithMain
        ring_buffer_lib
        )
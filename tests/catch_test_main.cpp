#include "catch2/catch_all.hpp"

#include "ringbuffer.hpp"


TEST_CASE("Test filling ringbuffer to capacity") {
    ringbuffer<int, 3> buff;
    buff.push(1);
    buff.push(2);
    buff.push(3);
    CHECK(buff[0] == 1);
    CHECK(buff[1] == 2);
    CHECK(buff[2] == 3);
}

TEST_CASE("Test creating filled ringbuffer from single value") {
    ringbuffer<int, 3> buff{42};
    CHECK(buff[0] == 42);
    CHECK(buff[1] == 42);
    CHECK(buff[2] == 42);
}


TEST_CASE("Test overwrite on buffer prefilled from single value") {
    ringbuffer<int, 3> buff{42};
    buff.push(9);
    CHECK(buff[0] == 9);
}


TEST_CASE("Test creating filled ringbuffer") {
    ringbuffer<int, 3> buff{1, 2, 3};
    CHECK(buff[0] == 1);
    CHECK(buff[1] == 2);
    CHECK(buff[2] == 3);
}

TEST_CASE("Test overwriting on push in buffer prefilled from multiple values") {
    ringbuffer<int, 3> buff{1, 2, 3};
    CHECK(buff[0] == 1);
    CHECK(buff[1] == 2);
    CHECK(buff[2] == 3);
    buff.push(99);
    CHECK(buff[0] == 99);
}

TEST_CASE("Test push in buffer prefilled only partially") {
    ringbuffer<int, 5> buff{1, 2, 3};
    CHECK(buff[0] == 1);
    CHECK(buff[1] == 2);
    CHECK(buff[2] == 3);
    buff.push(99);
    buff.push(100);
    CHECK(buff[3] == 99);
    CHECK(buff[4] == 100);
}

TEST_CASE("Test overwriting on push") {
    ringbuffer<int, 3> buff;
    buff.push(1);
    buff.push(2);
    buff.push(3);
    buff.push(99);
    buff.push(100);
    buff.push(101);
    CHECK(buff[0] == 99);
    CHECK(buff[1] == 100);
    CHECK(buff[2] == 101);

}

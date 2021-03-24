#include "catch2/catch_all.hpp"

#include "ringbuffer.hpp"


TEST_CASE("Test capacity and size from pushing") {
    ringbuffer<double, 5> a;
    CHECK(a.capacity() == 5);
    CHECK(a.empty());
    CHECK(a.size() == 0);

    SECTION("Pushing increases size") {
        a.push_back(1);
        CHECK(a.size() == 1);
        CHECK_FALSE(a.empty());
        a.push_back(2.5);
        CHECK(a.size() == 2);
    }

    SECTION("Push when at capacity doesn't increase size anymore") {
        for (int i = 0; i < a.capacity(); ++i) {
            a.push_back(0);
        }
        CHECK(a.size() == 5);
        a.push_back(1);
        CHECK(a.size() == 5);
    }
}


TEST_CASE("Test capacity and size of prefilled buffer") {
    // Test fully prefilled from multiple values
    ringbuffer<int, 4> buff_full{1, 2, 3, 4};
    CHECK(buff_full.size() == 4);
    // Test partially prefilled
    ringbuffer<int, 4> buff_half{1, 2};
    CHECK(buff_half.size() == 2);
    // Test fully prefilled from single value
    ringbuffer<int, 3> prefilled{7};
    CHECK(prefilled.size() == 3);
}


TEST_CASE("Reset clears buffer") {
    ringbuffer<int, 3> buff;
    buff.push_back(2);
    buff.clear();
    CHECK(buff.empty());
    buff.push_back(10);
    buff.push_back(20);
    buff.push_back(30);
    CHECK(buff[0] == 10);
    CHECK(buff[1] == 20);
    CHECK(buff[2] == 30);
    buff.clear();
    CHECK(buff.empty());
}


TEST_CASE("Test filling ringbuffer to capacity") {
    ringbuffer<int, 3> buff;
    buff.push_back(1);
    buff.push_back(2);
    buff.push_back(3);
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

TEST_CASE("Test creating filled ringbuffer") {
    ringbuffer<int, 3> buff{1, 2, 3};
    CHECK(buff[0] == 1);
    CHECK(buff[1] == 2);
    CHECK(buff[2] == 3);
}

TEST_CASE("Test creating partially filled ringbuffer") {
    ringbuffer<int, 5> buff{1, 2, 3};
    CHECK(buff[0] == 1);
    CHECK(buff[1] == 2);
    CHECK(buff[2] == 3);
}

TEST_CASE("Test overwrite on buffer prefilled from single value") {
    ringbuffer<int, 3> buff{42};
    buff.push_back(9);
    CHECK(buff[2] == 9);
}

TEST_CASE("Test overwriting on push_back in buffer prefilled from multiple values") {
    ringbuffer<int, 3> buff{1, 2, 3};
    CHECK(buff[0] == 1);
    CHECK(buff[1] == 2);
    CHECK(buff[2] == 3);
    buff.push_back(99);
    CHECK(buff[0] == 2);
    CHECK(buff[1] == 3);
    CHECK(buff[2] == 99);
}

TEST_CASE("Test push_back in buffer prefilled only partially") {
    ringbuffer<int, 5> buff{1, 2, 3};
    CHECK(buff[0] == 1);
    CHECK(buff[1] == 2);
    CHECK(buff[2] == 3);
    buff.push_back(99);
    buff.push_back(100);
    CHECK(buff[3] == 99);
    CHECK(buff[4] == 100);
}

TEST_CASE("Test overwriting on push_back in unfilled buffer") {
    ringbuffer<int, 3> buff;
    buff.push_back(1);
    buff.push_back(2);
    buff.push_back(3);
    buff.push_back(99);
    buff.push_back(100);
    buff.push_back(101);
    CHECK(buff[0] == 99);
    CHECK(buff[1] == 100);
    CHECK(buff[2] == 101);
}

TEST_CASE("Pop back on empty buffer works (no guarantuee for the value returned)") {
    ringbuffer<int, 3> buff;
    buff.pop_back();
}

TEST_CASE("Pop back without overwritten values") {
    ringbuffer<int, 3> buff{1, 2, 3};
    buff.pop_back();
    buff.pop_back();
    buff.pop_back();
    CHECK(buff.empty());
}

TEST_CASE("Pop back with overwritten values") {
    ringbuffer<int, 3> buff{1, 2, 3};
    buff.push_back(99);
    buff.pop_back();
    CHECK(buff.size() == 2);
    CHECK(buff[0] == 2);
    CHECK(buff[1] == 3);
    buff.pop_back();
    CHECK(buff.size() == 1);
    CHECK(buff[0] == 2);
    buff.pop_back();
    CHECK(buff.empty());
}

TEST_CASE("Pop back with repeatedly overwritten values") {
    ringbuffer<int, 3> buff{1, 2, 3};
    buff.push_back(10);
    buff.push_back(11);
    buff.push_back(12);
    buff.push_back(13);
    buff.pop_back();
    CHECK(buff.size() == 2);
    CHECK(buff[0] == 11);
    CHECK(buff[1] == 12);
    buff.pop_back();
    CHECK(buff.size() == 1);
    CHECK(buff[0] == 11);
    buff.pop_back();
    CHECK(buff.empty());
}

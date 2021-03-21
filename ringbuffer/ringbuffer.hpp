#pragma once

#include <initializer_list>
#include <array>


namespace impl {
    /**
     * Helper class to represent index of the circular buffer.
     * It implements a wrap around behavour between 0 and MAX.
     * Decrementing 0 will set the value to max, incrementing MAX
     * will set the value to 0.
     * @tparam MAX
     */
    template<size_t MAX>
    class Index {
    public:
        Index() : _index(0) {}
        Index(size_t val) : _index(val) { if (val > MAX) _index = 0; }

        void increment() {
            if (_index == MAX) {
                _index = 0;
            } else {
                ++_index;
            }
        }

        void decrement() {
            if (_index == 0) {
                _index = MAX;
            } else {
                --_index;
            }
        }

        operator size_t() {
            return _index;
        }

    private:
        size_t _index;
    };
}


template<typename T, size_t N>
class ringbuffer {
public:
    // Avoid headache/special cases for something which is inane anyways
    static_assert(N > 0, "Empty buffer is not allowed");

    /**
     * Creates uninitialized buffer.
     * Pushing will begin at index 0.
     */
    ringbuffer() = default;

    /**
     * Creates prefilled buffer.
     * Pushing will begin at index 0.
     * @param initial_value
     */
    ringbuffer(T initial_value): _index(0) {
        _buffer.fill(initial_value);
    }

    /**
     * Create prefilled buffer.
     * Pushing will begin past the first prefilled value or at zero if all values were prefilled.
     * @tparam U
     * @param u
     * @param ts
     */
    template<typename... U>
    ringbuffer(T u, U... ts) : _buffer{u, ts...}, _index(1 + sizeof...(ts)) {}

    /**
     * Push single value in the buffer.
     * @param value
     */
    void push_back(T value) {
        _buffer[_index] = value;
        _index.increment();
    }

    size_t capacity() {
        return N;
    }

    T &operator[](size_t i) {
        return _buffer[i];
    }

private:
    std::array<T, N> _buffer;
    // After every operation, this index should point to the next insertion point
    impl::Index<N - 1> _index;
};
#pragma once

#include <initializer_list>
#include <array>


namespace impl {
    /**
     * Helper class to represent index of the circular buffer.
     * It implements a wrap around behavour between 0 and MAX.
     * Decrementing 0 will set the value to max, incrementing MAX
     * will set the value to 0.
     * Assigning a value > MAX will set the value to value % MAX.
     * @tparam MAX
     */
    template<size_t MAX>
    class WrappingUnsignedInteger {
    public:
        WrappingUnsignedInteger() = default;
        explicit WrappingUnsignedInteger(size_t val) : _value(val) { if (val > MAX) _value %= MAX; }

        WrappingUnsignedInteger& operator=(size_t val) {
            _value = val;
            if (val > MAX) _value %= MAX;
            return *this;
        }

        void increment() {
            if (_value == MAX) {
                _value = 0;
            } else {
                ++_value;
            }
        }

        void decrement() {
            if (_value == 0) {
                _value = MAX;
            } else {
                --_value;
            }
        }

        operator size_t() const {
            return _value;
        }

    private:
        size_t _value = 0;
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
    explicit ringbuffer(T initial_value): _num_elements(N) {
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
    explicit ringbuffer(T u, U... ts) : _buffer{u, ts...}, _num_elements(1 + sizeof...(ts)) {}

    void clear() {
        _first = 0;
        _num_elements = 0;
    }

    /**
     * Push single value in the buffer.
     * @param value
     */
    void push_back(T value) {
        const auto i = (_first + _num_elements) % N;
        _buffer[i] = value;
        if (_num_elements == capacity()) {
            _first.increment();
        } else {
            ++_num_elements;
        }
    }

    /**
     * Remove last pushed value.
     * @return
     */
    void pop_back() {
        if (_num_elements == 0) {
            return;
        }
        --_num_elements;
    }

    [[nodiscard]] size_t capacity() const {
        return N;
    }

    [[nodiscard]] size_t size() const {
        return _num_elements;
    }

    [[nodiscard]] bool empty() const {
        return size() == 0;
    }

    /**
     * Get values in FIFO order. Index 0 will always be the oldest element of the container
     * @param i
     * @return
     */
    T &operator[](size_t i) {
        const auto j = (_first + i) % N;
        return _buffer[j];
    }

private:
    std::array<T, N> _buffer;
    // Points to the first element
    impl::WrappingUnsignedInteger<N - 1> _first;
    size_t _num_elements = 0;
};
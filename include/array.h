#pragma once

#include <memory_resource>
#include <initializer_list>
#include "allocator.h"
#include <iterator>

template <class T, class Alloc = std::pmr::polymorphic_allocator<T>>
class Array {
private:
    using traits = std::allocator_traits<Alloc>;
    size_t size = 0;
    size_t capacity = 0;
    T* data = nullptr;
    Alloc alloc;

public:
    class iterator {
        private:
            T* ptr;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            iterator(T* ptr = nullptr) : ptr(ptr) {}

            T& operator*() const { return *ptr; }
            T* operator->() const { return ptr; }

            iterator operator++(int) {
                iterator temporary = *this;
                ++ptr;
                return temporary;
            }
            iterator operator++() {
                ++ptr;
                return *this;
            }
            iterator operator+=(std::ptrdiff_t delta) {
                ptr += delta;
                return *this;
            }
            iterator operator+(std::ptrdiff_t delta) const {
                iterator temporary = *this;
                temporary += delta;
                return temporary;
            }

            iterator operator--(int) {
                iterator temporary = *this;
                --ptr;
                return temporary;
            }
            iterator operator--() {
                --ptr;
                return *this;
            }
            iterator operator-=(std::ptrdiff_t delta) {
                ptr -= delta;
                return *this;
            }
            iterator operator-(std::ptrdiff_t delta) const {
                iterator temporary = *this;
                temporary -= delta;
                return temporary;
            }

            std::ptrdiff_t operator-(const iterator& other) const { return ptr - other.ptr; }

            T& operator[](std::ptrdiff_t index) const { return *(ptr + index); }

            bool operator==(const iterator& other) const { return ptr == other.ptr; }
            bool operator!=(const iterator& other) const { return !(*this == other); }
            bool operator>(const iterator& other) const { return ptr > other.ptr; }
            bool operator<(const iterator& other) const { return ptr < other.ptr; }
            bool operator>=(const iterator& other) const { return ptr > other.ptr || ptr == other.ptr; }
            bool operator<=(const iterator& other) const { return ptr < other.ptr || ptr == other.ptr; }
    };


    class const_iterator {
        private:
            const T* ptr;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            const_iterator(const T* ptr = nullptr) : ptr(ptr) {}
            const_iterator(const const_iterator& other) : ptr(other.ptr) {}

            const T& operator*() const { return *ptr; }
            const T* operator->() const { return ptr; }

            const_iterator operator++(int) {
                const_iterator temporary = *this;
                ++ptr;
                return temporary;
            }
            const_iterator operator++() {
                ++ptr;
                return *this;
            }
            const_iterator operator+=(std::ptrdiff_t delta) {
                ptr += delta;
                return *this;
            }
            const_iterator operator+(std::ptrdiff_t delta) const {
                const_iterator temporary = *this;
                temporary += delta;
                return temporary;
            }

            const_iterator operator--(int) {
                const_iterator temporary = *this;
                --ptr;
                return temporary;
            }
            const_iterator operator--() {
                --ptr;
                return *this;
            }
            const_iterator operator-=(std::ptrdiff_t delta) {
                ptr -= delta;
                return *this;
            }
            const_iterator operator-(std::ptrdiff_t delta) const {
                const_iterator temporary = *this;
                temporary -= delta;
                return temporary;
            }

            std::ptrdiff_t operator-(const const_iterator& other) const { return ptr - other.ptr; }

            const T& operator[](std::ptrdiff_t index) const { return *(ptr + index); }

            bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
            bool operator!=(const const_iterator& other) const { return !(*this == other); }
            bool operator>(const const_iterator& other) const { return ptr > other.ptr; }
            bool operator<(const const_iterator& other) const { return ptr < other.ptr; }
            bool operator>=(const const_iterator& other) const { return ptr > other.ptr || ptr == other.ptr; }
            bool operator<=(const const_iterator& other) const { return ptr < other.ptr || ptr == other.ptr; }
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Array() = default;
    Array(const std::initializer_list<T>& list, const Alloc& alloc = Alloc()) : alloc(alloc) {
        reserve(list.size());
        std::uninitialized_copy(list.begin(), list.end(), data);
        this->size = list.size();
        this->capacity = size;
    }
    Array(size_t size, const T& value = T(), const Alloc& alloc = Alloc()) : alloc(alloc) {
        reserve(size);
        std::uninitialized_fill_n(data, size, value);
        this->size = size;
        this->capacity = size;
    }
    Array(const Array& other) : alloc(traits::select_on_container_copy_construction(other.alloc)) {
        reserve(other.capacity);
        std::copy(other.begin(), other.end(), data);
        this->size = other.size;
        this->capacity = other.capacity;
    }
    Array(Array&& other) noexcept : size(other.size), capacity(other.capacity), data(other.data), alloc(other.alloc) {
        other.size = 0;
        other.capacity = 0;
        other.data = nullptr;
    }
    ~Array() {
        clear();
        if (data)
            traits::deallocate(alloc, data, capacity);
    }
    
    Array& operator=(const Array& other) {
        if (this == &other) return *this;
        clear();
        resize(other.capacity);
        std::uninitialized_copy(other.begin(), other.end(), data);
        size = other.size;
        return *this;
    }
    Array& operator=(Array&& other) {
        if (this == &other) return *this;
        clear();
        traits::deallocate(alloc, data, capacity);
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        alloc = other.alloc;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
        return *this;
    }

    size_t getSize() {
        return size;
    }
    T& operator[](size_t index) {
        return data[index];
    }
    const T& operator[](size_t index) const {
        return data[index];
    }
    void set(std::ptrdiff_t index, const T& value) {
        if (size == 0) throw std::runtime_error("Array is empty");
        if (index < 0) index += static_cast<std::ptrdiff_t>(size);
        if (index < 0 || index >= static_cast<std::ptrdiff_t>(size)) throw std::out_of_range("Index out of range");
        data[index] = value;
    }
    void push_back(const T& value) {
        if (size >= capacity) reserve((capacity == 0)? 1 : capacity * 2);
        traits::construct(alloc, data + size, value);
        ++size;
    }
    T remove(int index) {
        if (size == 0) throw std::runtime_error("Array is empty");
        if (index < 0) index += size;
        if (index < 0 || index >= size) throw std::out_of_range("Index out of range");

        T temp = data[index];
        std::move(begin() + index + 1, end(), begin() + index);
        traits::destroy(alloc, data + --size);
        return temp;
    }
    void resize(size_t newSize) {
        if (newSize > capacity) reserve(newSize);
        for (size_t i = size; i < newSize; ++i)
            traits::construct(alloc, data + i);
        for (size_t i = newSize; i < size; ++i)
            traits::destroy(alloc, data + i);
        size = newSize;
    }
    void clear() {
        for (size_t i = 0; i < size; ++i)
            traits::destroy(alloc, data + i);
        size = 0;
    }
    T pop_back() {
        return remove(-1);
    }
    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity) return;
        T* newData = traits::allocate(alloc, newCapacity);
        for (size_t i = 0; i < size; ++i) {
            traits::construct(alloc, newData + i, std::move(data[i]));
            traits::destroy(alloc, data + i);
        }
        if (data)
            traits::deallocate(alloc, data, capacity);
        data = newData;
        capacity = newCapacity;
    }

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size); }

    const_iterator begin() const noexcept { return const_iterator(data); }
    const_iterator end() const noexcept { return const_iterator(data + size); }

    const_iterator cbegin() const noexcept { return const_iterator(data); }
    const_iterator cend() const noexcept { return const_iterator(data + size); }

    std::reverse_iterator<iterator> rbegin() noexcept { return std::reverse_iterator<iterator>(end()); }
    std::reverse_iterator<iterator> rend() noexcept { return std::reverse_iterator<iterator>(begin()); }

    std::reverse_iterator<const_iterator> rbegin() const noexcept { return std::reverse_iterator<const_iterator>(end()); }
    std::reverse_iterator<const_iterator> rend() const noexcept { return std::reverse_iterator<const_iterator>(begin()); }
    
    std::reverse_iterator<const_iterator> crbegin() const noexcept { return std::reverse_iterator<const_iterator>(end()); }
    std::reverse_iterator<const_iterator> crend() const noexcept { return std::reverse_iterator<const_iterator>(begin()); }

    
};


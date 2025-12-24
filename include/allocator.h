#pragma once

#include <cstddef>
#include <memory_resource>
#include <list>
#include <algorithm>


class Allocator : public std::pmr::memory_resource {
private:
    struct Block {
        void* ptr;
        size_t size;
        size_t alignment;
        bool isFree;
    };

    std::list<Block> blocks;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override {
        if (bytes == 0) return nullptr;
        for (auto& b : blocks) {
            if (b.isFree && b.size >= bytes && b.alignment >= alignment) {
                b.isFree = false;
                return b.ptr;
            }
        }
        void* ptr = ::operator new(bytes, std::align_val_t(alignment));
        blocks.push_back({ptr, bytes, alignment, false});
        return ptr;
    }

    void do_deallocate(void* ptr, size_t bytes, size_t alignment) override {
        if (!ptr) return;
        for (auto& b : blocks) {
            if (b.ptr == ptr) {
                b.isFree = true;
                return;
            }
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

public:
    Allocator() = default;

    ~Allocator() noexcept {
        for (auto& block : blocks) {
            ::operator delete(block.ptr, std::align_val_t(block.alignment));
        }
        blocks.clear();
    }

    Allocator(const Allocator& allocator) = delete;
    Allocator(Allocator&& allocator) = delete;
    Allocator& operator=(const Allocator& allocator) = delete;
    Allocator& operator=(Allocator&& allocator) = delete;

};




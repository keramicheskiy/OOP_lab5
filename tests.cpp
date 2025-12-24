#include <gtest/gtest.h>

#include "allocator.h"
#include <memory_resource>
#include "array.h"
#include "complex_struct.h"
#include <algorithm>


TEST(Alloc, AllocationAndDeallocation) {
    Allocator allocator{};
    void* p1 = allocator.allocate(10, 16);
    void* p2 = allocator.allocate(11, 32);
    EXPECT_NE(p1, nullptr);
    EXPECT_NE(p2, nullptr);
    allocator.deallocate(p1, 10, 16);
    allocator.deallocate(p2, 11, 32);
}

TEST(Alloc, Reusage) {
    Allocator allocator{};
    void* p1 = allocator.allocate(128, 8);
    allocator.deallocate(p1, 128, 8);
    void* p2 = allocator.allocate(64, 8);
    EXPECT_EQ(p1, p2);
}

TEST(Allocator, AlignmentCheck) {
    Allocator allocator{};

    void* p16 = allocator.allocate(10, 16);
    void* p32 = allocator.allocate(10, 32);
    void* p64 = allocator.allocate(10, 64);

    EXPECT_EQ(reinterpret_cast<uintptr_t>(p16) % 16, 0u);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(p32) % 32, 0u);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(p64) % 64, 0u);
}

TEST(Allocator, ArrayInitialization) {
    Allocator allocator{};
    std::pmr::polymorphic_allocator<int> intPolymorphicAllocator(&allocator);
    Array<int, std::pmr::polymorphic_allocator<int>> intArray({1, 2, 6}, intPolymorphicAllocator);
    int arr[] = {1, 2, 6};
    for (int i = 0; i < intArray.getSize(); ++i) {
        EXPECT_EQ(intArray[i], arr[i]);
    }
}

TEST(Allocator, ArrayRemove) {
    Allocator allocator{};
    std::pmr::polymorphic_allocator<int> intPolymorphicAllocator(&allocator);
    Array<int, std::pmr::polymorphic_allocator<int>> intArray({1, 2, 6}, intPolymorphicAllocator);
    int arr[] = {1, 6};
    intArray.remove(1);
    for (int i = 0; i < intArray.getSize(); ++i) {
        EXPECT_EQ(intArray[i], arr[i]);
    }
}


TEST(Allocator, ArrayPushBack) {
    Allocator allocator{};
    std::pmr::polymorphic_allocator<int> intPolymorphicAllocator(&allocator);
    Array<int, std::pmr::polymorphic_allocator<int>> intArray({1, 2, 6}, intPolymorphicAllocator);
    int arr[] = {1, 2, 6, 67};
    intArray.push_back(67);
    for (int i = 0; i < intArray.getSize(); ++i) {
        EXPECT_EQ(intArray[i], arr[i]);
    }
}

TEST(Allocator, ArraySet) {
    Allocator allocator{};
    std::pmr::polymorphic_allocator<int> intPolymorphicAllocator(&allocator);
    Array<int, std::pmr::polymorphic_allocator<int>> intArray({1, 2, 6}, intPolymorphicAllocator);
    int arr[] = {1, 3, 6};
    intArray.set(1, 3);
    for (int i = 0; i < intArray.getSize(); ++i) {
        EXPECT_EQ(intArray[i], arr[i]);
    }
}

TEST(Allocator, ArrayResize) {
    Allocator allocator{};
    std::pmr::polymorphic_allocator<int> intPolymorphicAllocator(&allocator);
    Array<int, std::pmr::polymorphic_allocator<int>> intArray({1, 2, 6}, intPolymorphicAllocator);
    intArray.resize(30);
    EXPECT_EQ(intArray.getSize(), 30);
}

TEST(Allocator, ArrayClear) {
    Allocator allocator{};
    std::pmr::polymorphic_allocator<int> intPolymorphicAllocator(&allocator);
    Array<int, std::pmr::polymorphic_allocator<int>> intArray({1, 2, 6}, intPolymorphicAllocator);
    intArray.clear();
    EXPECT_EQ(intArray.getSize(), 0);

    EXPECT_EQ(intArray.begin(), intArray.end());
}


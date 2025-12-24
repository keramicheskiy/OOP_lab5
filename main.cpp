#include <iostream>
#include "allocator.h"
#include <memory_resource>
#include "array.h"
#include "complex_struct.h"
#include <algorithm>


int main() {
    Allocator allocator{};

    std::pmr::polymorphic_allocator<int> intPolymorphicAllocator(&allocator);
    std::pmr::polymorphic_allocator<int> complexStructurePolymorphicAllocator(&allocator);

    Array<int, std::pmr::polymorphic_allocator<int>> intArray({1, 2, 6}, intPolymorphicAllocator);

    Array<complex_structure, std::pmr::polymorphic_allocator<complex_structure>> 
        structArray({
            {{1, 1}, "one"},
            {{1, 3, 88}, "two"},
            {{8, 8, 1}, "three"},
            {{}, ""}
        }, complexStructurePolymorphicAllocator);
    
    for (auto x : intArray) { std::cout << x << " "; }
    std::cout << std::endl << "Enter new values" << std::endl;
    for (auto& x : intArray) { std::cin >> x; }
    for (auto x : intArray) { std::cout << x << " "; }
    std::cout << std::endl;
    std::cout << "Sorted" << std::endl;
    std::sort(intArray.begin(), intArray.end());
    for (auto x : intArray) { std::cout << x << " "; }
    std::cout << std::endl;

    for (auto x : structArray) { std::cout << x << std::endl; }
    structArray.pop_back();
    std::cout << "Removed last element" << std::endl;
    for (auto x : structArray) { std::cout << x << std::endl; }
    structArray.set(0, {{9, 9, 9, 9, 9, 9, 9, 9000}, "stop the torture"});
    std::cout << "Replaced first element" << std::endl;
    for (auto x : structArray) { std::cout << x << std::endl; }
    std::reverse(structArray.begin(), structArray.end());
    std::cout << "Reversed" << std::endl;
    for (auto x : structArray) { std::cout << x << std::endl; }

}
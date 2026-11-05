#include <iostream>

#include "allocator/memory_pool.hpp"

int main() {
    std::cout << "Custom Memory Allocator" << std::endl;
    std::cout << "Phase 1 - Memory Fundamentals" << std::endl;

    allocator::MemoryPool pool(1024);
    void* block = pool.allocate(64);
    pool.deallocate(block);

    return 0;
}

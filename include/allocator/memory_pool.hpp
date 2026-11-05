#pragma once

#include <cstddef>
#include <vector>

namespace allocator {

class MemoryPool {
public:
    // Phase 1 uses a fixed allocation-unit size. Arbitrary sizes arrive in Phase 3.
    static constexpr std::size_t kAlignment = 16;
    static constexpr std::size_t kUnitSize = 16;

    explicit MemoryPool(std::size_t capacity);
    ~MemoryPool();

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    void* allocate(std::size_t size);
    void deallocate(void* ptr);

    [[nodiscard]] std::size_t capacity() const;
    [[nodiscard]] std::size_t used() const;
    [[nodiscard]] std::size_t available() const;

private:
    struct Allocation {
        void* start = nullptr;
        std::vector<void*> blocks;
    };

    std::size_t capacity_;
    std::size_t used_;
    void* region_;
    std::vector<void*> free_units_;
    std::vector<Allocation> allocations_;
};

}  // namespace allocator

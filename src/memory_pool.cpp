#include "allocator/memory_pool.hpp"

#include <stdexcept>

namespace allocator
{

    MemoryPool::MemoryPool(std::size_t capacity)
        : capacity_(capacity), used_(0), region_(nullptr)
    {
        const std::size_t aligned_capacity =
            (capacity_ + kAlignment - 1) / kAlignment * kAlignment;
        region_ = new std::byte[aligned_capacity];

        const std::size_t num_units = capacity_ / kUnitSize;
        free_units_.reserve(num_units);
        for (std::size_t i = 0; i < num_units; ++i)
        {
            free_units_.push_back(static_cast<std::byte *>(region_) + i * kUnitSize);
        }
    }

    MemoryPool::~MemoryPool()
    {
        delete[] static_cast<std::byte *>(region_);
        region_ = nullptr;
        free_units_.clear();
        allocations_.clear();
    }

    void *MemoryPool::allocate(std::size_t size)
    {
        if (region_ == nullptr)
        {
            return nullptr;
        }

        std::size_t units = (size + kUnitSize - 1) / kUnitSize;
        if (units == 0)
        {
            units = 1;
        }

        if (free_units_.size() < units)
        {
            return nullptr;
        }

        std::vector<void *> blocks;
        blocks.reserve(units);
        for (std::size_t i = 0; i < units; ++i)
        {
            blocks.push_back(free_units_.back());
            free_units_.pop_back();
        }

        Allocation allocation;
        allocation.start = blocks.front();
        allocation.blocks = std::move(blocks);
        allocations_.push_back(allocation);

        used_ += units * kUnitSize;

        return allocations_.back().start;
    }

    void MemoryPool::deallocate(void *ptr)
    {
        if (ptr == nullptr)
        {
            return;
        }

        for (std::size_t i = 0; i < allocations_.size(); ++i)
        {
            if (allocations_[i].start == ptr)
            {
                std::vector<void *> blocks = std::move(allocations_[i].blocks);
                allocations_.erase(allocations_.begin() + static_cast<std::ptrdiff_t>(i));
                for (void *block : blocks)
                {
                    free_units_.push_back(block);
                }
                used_ -= static_cast<std::size_t>(blocks.size()) * kUnitSize;
                return;
            }
        }

        throw std::invalid_argument("MemoryPool::deallocate: invalid pointer");
    }

    std::size_t MemoryPool::capacity() const
    {
        return capacity_;
    }

    std::size_t MemoryPool::used() const
    {
        return used_;
    }

    std::size_t MemoryPool::available() const
    {
        return capacity_ - used_;
    }

}

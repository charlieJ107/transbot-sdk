#include "memory_pool.hpp"
#include "glog/logging.h"

#include <cstring>

#include <algorithm>
void MemoryPool::free(MemoryBlock *memory_block)
{
    if (memory_block == (MemoryBlock *) nullptr || !memory_block->in_use || memory_block->memory_ptr == nullptr)
    {
        // The memory block is not in use
        return;
    }
    if (memory_block->index_in_pool == 0 ||
        memory_block->index_in_pool == (blocks_nums_in_table - 1))
    {
        // The first block and the last block is always in use and should not be freed
        return;
    }
    memory_block->in_use = false;
}

MemoryBlock *MemoryPool::alloc(uint16_t size)
{

    // If size is larger than memory size or max size for pool, allocate failed
    if (size > block_max_size || size > pool_memory_size)
    {
        LOG(ERROR) << "Memory size is too large, memory size: " << size
                   << ", max memory size: " << block_max_size
                   << ", memory pool size: " << pool_memory_size;
        return nullptr;
    }

    // Calculate the in_use memory size and get the in_use index array
    uint16_t memory_size_in_use = 0;
    uint16_t in_use_blocks_num = 0;
    uint16_t* index_of_blocks_in_use = new uint16_t[blocks_nums_in_table];
    for (uint16_t i = 0; i < blocks_nums_in_table; i++)
    {
        if (blocks_table[i].in_use)
        {
            memory_size_in_use += blocks_table[i].memory_size;
            index_of_blocks_in_use[in_use_blocks_num++] = blocks_table[i].index_in_pool;
        }
    }

    // If left size is smaller than needed, allocate failed
    if (pool_memory_size < (memory_size_in_use + size))
    {
        LOG(ERROR) << "Memory pool is full, memory size: " << pool_memory_size
                   << ", memory size in use: " << memory_size_in_use
                   << ", memory size needed: " << size;
        return nullptr;
    }

    // Special case: allocate for the first time (Only the first and last block is in use, but their size is
    // always 0). therefor, if memory_size_in_use == 0, it means the memory pool is empty
    if (memory_size_in_use == 0)
    {
        blocks_table[1].memory_ptr = blocks_table[0].memory_ptr;
        blocks_table[1].memory_size = size;
        blocks_table[1].in_use = true;
        return &blocks_table[1];
    }

    // memory_table is out of order, memory_table_used_index is ordered by its memory allocation order
    // TODO: optimize the sort algorithm
    for (uint16_t i = 0; i < (in_use_blocks_num - 1); i++)
    {
        for (int j = 0; j < (in_use_blocks_num - i - 1); j++)
        {
            if (blocks_table[index_of_blocks_in_use[j]].memory_ptr >
                blocks_table[index_of_blocks_in_use[j + 1]].memory_ptr)
            {
                index_of_blocks_in_use[j + 1] ^= index_of_blocks_in_use[j];
                index_of_blocks_in_use[j] ^= index_of_blocks_in_use[j + 1];
                index_of_blocks_in_use[j + 1] ^= index_of_blocks_in_use[j];
            }
        }
    }


    // The current smallest memory space available for the needed size
    uint16_t smallest_available_memory_space = pool_memory_size;
    // The index of the in_use block right before the current smallest memory space available for the needed size
    // Which means the memory space is smallest and enough for the needed size at:
    // blocks_table[index_of_in_use_block_before_smallest_available_memory_space].memory_ptr +
    // blocks_table[index_of_in_use_block_before_smallest_available_memory_space].memory_size
    uint16_t index_of_in_use_block_before_smallest_available_memory_space = blocks_nums_in_table - 1;
    // The accumulated available memory space
    uint16_t accumulated_available_memory_space = 0;

    bool free_space_enough_for_needed_size = false;
    int index_when_free_space_enough_for_needed_size = 0;
    // Check each space between two in_use blocks, find the smallest available memory space for the needed size,
    // and the index of the in_use block right before that smallest available memory space.
    // If there are more than one smallest available memory space, the one with the largest index will be chosen.
    // Also calculate the accumulated available memory space, if there is no available memory space for the needed
    // size (which means the smallest_available_memory_space is still pool_memory_size and
    // the index_of_in_use_block_before_smallest_available_memory_space is still blocks_nums_in_table - 1 after the
    // loop), but the accumulated available memory space is enough for the needed size, we will "compress" the
    // memory pool, and allocate the memory block in the compressed memory pool.
    for (uint16_t i = 0; i < (in_use_blocks_num - 1); i++)
    {
        // The memory space between two in_use blocks
        auto free_memory_space_between_blocks = static_cast<uint32_t>(
                blocks_table[index_of_blocks_in_use[i + 1]].memory_ptr -
                blocks_table[index_of_blocks_in_use[i]].memory_ptr);

        // The space between two in_use blocks is enough for the needed size
        if ((free_memory_space_between_blocks - blocks_table[index_of_blocks_in_use[i]].memory_size) >= size &&
            smallest_available_memory_space >
            (free_memory_space_between_blocks - blocks_table[index_of_blocks_in_use[i]].memory_size))
        {
            index_of_in_use_block_before_smallest_available_memory_space =
                    blocks_table[index_of_blocks_in_use[i]].index_in_pool;
            smallest_available_memory_space =
                    free_memory_space_between_blocks - blocks_table[index_of_blocks_in_use[i]].memory_size;
        }

        // accumulate the free memory size for each block
        accumulated_available_memory_space +=
                free_memory_space_between_blocks - blocks_table[index_of_blocks_in_use[i]].memory_size;
        // record the index when accumulate_left_memory_size is larger than needed size for the first time
        if (accumulated_available_memory_space >= size && !free_space_enough_for_needed_size)
        {
            index_when_free_space_enough_for_needed_size = i;
            free_space_enough_for_needed_size = true;
        }
    }

    // If no single block is available to divide for needed size, then compress the table according to accumulate
    // memory, and allocate the memory block in the compressed memory pool.
    if (index_of_in_use_block_before_smallest_available_memory_space == blocks_nums_in_table &&
        smallest_available_memory_space == pool_memory_size)
    {
        for (int i = 0; i < index_when_free_space_enough_for_needed_size; i++)
        {
            if (blocks_table[index_of_blocks_in_use[i + 1]].memory_ptr >
                (blocks_table[index_of_blocks_in_use[i]].memory_ptr +
                 blocks_table[index_of_blocks_in_use[i]].memory_size))
            {
                // move the memory block to the left, right after the previous block
                memmove(blocks_table[index_of_blocks_in_use[i]].memory_ptr +
                        blocks_table[index_of_blocks_in_use[i]].memory_size, // destination
                        blocks_table[index_of_blocks_in_use[i + 1]].memory_ptr, // source
                        blocks_table[index_of_blocks_in_use[i + 1]].memory_size); // size
                // update the memory pointer of the moved block
                blocks_table[index_of_blocks_in_use[i + 1]].memory_ptr =
                        blocks_table[index_of_blocks_in_use[i]].memory_ptr +
                        blocks_table[index_of_blocks_in_use[i]].memory_size;
            }
        }

        for (uint16_t i = 1; i < (blocks_nums_in_table - 1); i++)
        {
            if (!blocks_table[i].in_use)
            {
                // Find a free block, and allocate the set the free space as memory of this free block
                blocks_table[i].memory_ptr =
                        blocks_table[
                                index_of_blocks_in_use[index_when_free_space_enough_for_needed_size]
                        ].memory_ptr +
                        blocks_table[
                                index_of_blocks_in_use[index_when_free_space_enough_for_needed_size]
                        ].memory_size;

                blocks_table[i].memory_size = size;
                blocks_table[i].in_use = true;
                return &blocks_table[i];
            }
        }
        delete[] index_of_blocks_in_use;
        return nullptr;
    }

    //If single block is available to divide for needed size, then divide this block into two
    for (uint16_t i = 1; i < (blocks_nums_in_table - 1); i++)
    {
        if (!blocks_table[i].in_use)
        {
            blocks_table[i].memory_ptr =
                    blocks_table[index_of_in_use_block_before_smallest_available_memory_space].memory_ptr +
                    blocks_table[index_of_in_use_block_before_smallest_available_memory_space].memory_size;

            blocks_table[i].memory_size = size;
            blocks_table[i].in_use = true;
            return &blocks_table[i];
        }
    }

    return nullptr;
}


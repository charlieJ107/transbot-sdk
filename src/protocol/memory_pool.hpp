#ifndef TRANSBOT_SDK_MEMORY_POOL_HPP
#define TRANSBOT_SDK_MEMORY_POOL_HPP
#include <cstdint>
#include <mutex>
/**
 * @brief Memory block information
 */
typedef struct MemoryBlock
{
    //! flag of usage, ture if this block is in use
    bool in_use;
    //! memory block index in the pool
    uint8_t index_in_pool;
    //! size of in_use memory in this block
    uint16_t memory_size;
    //! memory pointer
    uint8_t *memory_ptr;
} MemoryBlock;

/**
 * @brief Memory pool to manage memory blocks
 */
class MemoryPool
{
public:
    /**
     * @brief Constructor of memory pool
     * @param max_block_size Max size for single memory block
     * @param memory_size Max memory size
     * @param memory_table_num Memory block number in the table
     */
    explicit MemoryPool(uint16_t max_block_size = 1024,
                        uint16_t memory_size = 1024,
                        uint16_t memory_table_num = 32) : blocks_nums_in_table(memory_table_num),
                                                          pool_memory_size(memory_size),
                                                          block_max_size(max_block_size)
    {
        blocks_table = new MemoryBlock[blocks_nums_in_table];
        pool_memory_ptr = new uint8_t[pool_memory_size];

        // Initialize memory first block in the pool
        blocks_table[0].memory_ptr = pool_memory_ptr;
        blocks_table[0].index_in_pool = 0;
        blocks_table[0].memory_size = 0;
        blocks_table[0].in_use = true; // The first block is always in use
        // Initialize memory last block in the pool
        // The last block memory pointer is point to the end of the pool
        blocks_table[blocks_nums_in_table - 1].memory_ptr = pool_memory_ptr + pool_memory_size;
        blocks_table[blocks_nums_in_table - 1].index_in_pool = blocks_nums_in_table - 1;
        blocks_table[blocks_nums_in_table - 1].memory_size = 0;
        blocks_table[blocks_nums_in_table - 1].in_use = true; // The last block is always in use

        // Initialize memory other blocks in the pool
        for (int i = 1; i < (blocks_nums_in_table - 1); ++i)
        {
            // Initialize memory pointer with nullptr, will be set when allocate memory
            blocks_table[i].memory_ptr = nullptr;
            blocks_table[i].index_in_pool = i;
            blocks_table[i].memory_size = 0;
            blocks_table[i].in_use = false;
        }
    };

    /**
     * @brief Destructor of memory pool
     */
    ~MemoryPool()
    {
        delete[] pool_memory_ptr;
        delete[] blocks_table;
    }

    /**
     * @brief Free certain memory block in the pool
     * @param memory_block Memory block to be freed
     */
    void free(MemoryBlock *memory_block);

    /**
     * @brief Allocate a memory block in the pool
     * @param size size of the memory block
     * @return pointer of memory block
     */
    MemoryBlock *alloc(uint16_t size);

private:
    //! mutex of the memory pool
    std::mutex mutex;
    //! number of memory block in the table
    uint16_t blocks_nums_in_table;
    //! max memory size in pool
    uint16_t pool_memory_size;
    //! max size for single memory block
    uint16_t block_max_size;
    //! memory table of blocks in pool
    MemoryBlock *blocks_table;
    //! memory pointer
    uint8_t *pool_memory_ptr;
};

#endif // TRANSBOT_SDK_MEMORY_POOL_HPP

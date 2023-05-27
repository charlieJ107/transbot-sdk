#ifndef TRANSBOT_SDK_CIRCULAR_BUFFER_HPP
#define TRANSBOT_SDK_CIRCULAR_BUFFER_HPP

#include <memory>
#include <mutex>
/**
 * @brief CircularBuffer for certain command
 * @tparam T Certain Data type in_use in the buffer
 */
template <class T>
class CircularBuffer {
public:
    /**
     * @brief Constructor of circular buffer
     * @param size
     */
    explicit CircularBuffer(size_t size) :
            buffer(std::unique_ptr<T[]>(new T[size])),
            max_size(size)
    {}
    /**
     * @brief push the data into the buffer
     * @param item Item to be pushed into the buffer
     */
    void push(T item)
    {
        std::lock_guard<std::mutex> lock(mutex);

        buffer[head] = item;

        if(full)
        {
            tail = (tail + 1) % max_size;
        }

        head = (head + 1) % max_size;

        full = head == tail;
    }

    /**
     * @brief pop the data from the buffer and get the popped item
     * @param item Item to be popped from the buffer
     * @return True if buffer is not empty
     */
    T pop()
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (is_empty())
        {
            throw std::length_error("Buffer is empty");
        }

        //Read data and advance the tail (we now have a free space)
        auto item = buffer[tail];
        full = false;
        tail = (tail + 1) % max_size;

        return item;
    }
    /**
     * @brief reset the buffer
     */
    void reset()
    {
        std::lock_guard<std::mutex> lock(mutex);
        head = tail;
        full = false;
    }
    /**
     * @brief Decide whether the buffer is empty
     * @return True if empty
     */
    bool is_empty() const
    {
        //if head and tail are equal, we are empty
        return (!full && (head == tail));
    }
    /**
     * @brief Decide whether the buffer is full
     * @return True if full
     */
    bool is_full() const
    {
        //If tail is ahead the head by 1, we are full
        return full;
    }
    /**
     * @brief Get the buffer capacity(max size of the buffer)
     * @return The buffer capacity
     */
    size_t get_capacity() const
    {
        return max_size;
    }
    /**
     * @brief Get the current size of the buffer
     * @return The current size of the buffer
     */
    size_t size() const
    {
        size_t size = max_size;

        if(!full)
        {
            if(head >= tail)
            {
                size = head - tail;
            }
            else
            {
                size = max_size + head - tail;
            }
        }

        return size;
    }

private:
    //! mutex of the buffer
    std::mutex mutex;
    //! buffer pointer
    std::unique_ptr<T[]> buffer;
    //! buffer head
    size_t head = 0;
    //! buffer tail
    size_t tail = 0;
    //! buffer capacity
    size_t max_size;
    //! flag of full buffer
    bool full = 0;
};

#endif //TRANSBOT_SDK_CIRCULAR_BUFFER_HPP

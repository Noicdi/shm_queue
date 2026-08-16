#ifndef SHM_QUEUE_SPSC_QUEUE_HPP
#define SHM_QUEUE_SPSC_QUEUE_HPP

#include <cstddef>

#include <type_traits>

#include "shm_queue/spsc_queue.h"

namespace shm_queue {

template <typename T>
class SpscQueue {
  public:
    static_assert(std::is_trivially_copyable_v<T>, "SpscQueue<T> requires T to be trivially copyable");

    SpscQueue() = default;
    SpscQueue(const SpscQueue&) = delete;
    SpscQueue& operator=(const SpscQueue&) = delete;

    SpscQueue(SpscQueue&& other) noexcept
        : queue_(other.queue_) {
        other.queue_ = nullptr;
    }

    SpscQueue& operator=(SpscQueue&& other) noexcept {
        if (this != &other) {
            if (queue_ != nullptr) {
                shm_queue_spsc_close(queue_);
            }

            queue_ = other.queue_;
            other.queue_ = nullptr;
        }

        return *this;
    }

    ~SpscQueue() {
        if (queue_ != nullptr) {
            shm_queue_spsc_close(queue_);
        }
    }

    // producer
    bool Create(const char* name, std::size_t capacity) {
        if (queue_ != nullptr) {
            return false;
        }

        queue_ = shm_queue_spsc_create(name, sizeof(T), capacity);
        return queue_ != nullptr;
    }
    // copy
    bool Push(const T& data) {
        return shm_queue_spsc_push(queue_, &data) == 0;
    }
    // zero-copy
    T* AcquireWrite() {
        return static_cast<T*>(shm_queue_spsc_acquire_write(queue_));
    }
    bool CommitWrite() const {
        return shm_queue_spsc_commit_write(queue_) == 0;
    }

    // consumer
    bool Attach(const char* name) {
        if (queue_ != nullptr) {
            return false;
        }

        queue_ = shm_queue_spsc_attach(name, sizeof(T));
        return queue_ != nullptr;
    }
    // copy
    bool Pop(T& data) {
        return shm_queue_spsc_pop(queue_, &data) == 0;
    }
    // zero-copy
    const T* AcquireRead() {
        return static_cast<const T*>(shm_queue_spsc_acquire_read(queue_));
    }
    bool ReleaseRead() const {
        return shm_queue_spsc_release_read(queue_) == 0;
    }

    // common
    static bool Destroy(const char* name) {
        return shm_queue_spsc_destroy(name) == 0;
    }

  private:
    shm_queue_spsc_t* queue_{nullptr};
};

}  // namespace shm_queue

#endif  // SHM_QUEUE_SPSC_QUEUE_HPP

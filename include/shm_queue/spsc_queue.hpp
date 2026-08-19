#ifndef SHM_QUEUE_SPSC_QUEUE_HPP
#define SHM_QUEUE_SPSC_QUEUE_HPP

#include <cstddef>

#include <type_traits>

#include "shm_queue/spsc_queue.h"

namespace shm_queue {

template <typename T>
class spsc_queue {
  public:
    static_assert(std::is_trivially_copyable<T>::value, "shm_queue::spsc_queue<T> requires T to be trivially copyable");

    spsc_queue() = default;
    spsc_queue(const spsc_queue&) = delete;
    spsc_queue& operator=(const spsc_queue&) = delete;

    spsc_queue(spsc_queue&& other) noexcept
        : queue_(other.queue_) {
        other.queue_ = nullptr;
    }

    spsc_queue& operator=(spsc_queue&& other) noexcept {
        if (this != &other) {
            if (queue_ != nullptr) {
                shm_queue_spsc_close(queue_);
            }

            queue_ = other.queue_;
            other.queue_ = nullptr;
        }

        return *this;
    }

    ~spsc_queue() {
        if (queue_ != nullptr) {
            shm_queue_spsc_close(queue_);
        }
    }

    // producer
    bool create(const char* name, std::size_t capacity) {
        if (queue_ != nullptr) {
            return false;
        }

        queue_ = shm_queue_spsc_create(name, sizeof(T), capacity);
        return queue_ != nullptr;
    }
    // copy
    bool push(const T& data) {
        return shm_queue_spsc_push(queue_, &data) == 0;
    }
    // zero-copy
    T* acquire_write() {
        return static_cast<T*>(shm_queue_spsc_acquire_write(queue_));
    }
    bool commit_write() const {
        return shm_queue_spsc_commit_write(queue_) == 0;
    }

    // consumer
    bool attach(const char* name) {
        if (queue_ != nullptr) {
            return false;
        }

        queue_ = shm_queue_spsc_attach(name, sizeof(T));
        return queue_ != nullptr;
    }
    // copy
    bool pop(T& data) {
        return shm_queue_spsc_pop(queue_, &data) == 0;
    }
    // zero-copy
    const T* acquire_read() {
        return static_cast<const T*>(shm_queue_spsc_acquire_read(queue_));
    }
    bool release_read() const {
        return shm_queue_spsc_release_read(queue_) == 0;
    }

    // common
    static bool destroy(const char* name) {
        return shm_queue_spsc_destroy(name) == 0;
    }

  private:
    shm_queue_spsc_t* queue_{nullptr};
};

}  // namespace shm_queue

#endif  // SHM_QUEUE_SPSC_QUEUE_HPP

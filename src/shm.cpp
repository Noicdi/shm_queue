#include "shm.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void* ShmCreate(const char* name, const std::size_t size) {
    const int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return nullptr;
    }

    if (ftruncate(fd, static_cast<off_t>(size)) == -1) {
        close(fd);
        return nullptr;
    }

    void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    if (addr == MAP_FAILED) {
        return nullptr;
    }

    return addr;
}

void* ShmAttach(const char* name, const std::size_t size) {
    const int fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return nullptr;
    }

    void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    if (addr == MAP_FAILED) {
        return nullptr;
    }

    return addr;
}

bool ShmClose(void* addr, std::size_t size) {
    return munmap(addr, size) == 0;
}

bool ShmDestroy(const char* name) {
    return shm_unlink(name) == 0;
}

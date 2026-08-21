#include <cerrno>
#include <cstddef>
#include <cstdlib>

#include <string>

#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include "shm.h"

namespace {

bool WriteStatus(const int fd, const unsigned char status) {
    ssize_t result;
    do {
        result = write(fd, &status, sizeof(status));
    } while (result == -1 && errno == EINTR);

    return result == sizeof(status);
}

bool ReadStatus(const int fd, unsigned char* const status) {
    ssize_t result;
    do {
        result = read(fd, status, sizeof(*status));
    } while (result == -1 && errno == EINTR);

    return result == sizeof(*status);
}

}  // namespace

TEST(ShmTest, CreateCloseAndDestroy) {
    constexpr std::size_t k_shm_size = 4096;
    const std::string shm_name = "/shm_test_" + std::to_string(getpid());

    void* const address = ShmCreate(shm_name.c_str(), k_shm_size);
    ASSERT_NE(address, nullptr);

    EXPECT_TRUE(ShmClose(address, k_shm_size));
    EXPECT_TRUE(ShmDestroy(shm_name.c_str()));
}

TEST(ShmTest, SingleProcessWriteAndRead) {
    constexpr std::size_t k_shm_size = 4096;
    const std::string shm_name = "/shm_test_" + std::to_string(getpid());

    void* const address = ShmCreate(shm_name.c_str(), k_shm_size);
    ASSERT_NE(address, nullptr);

    int* test = static_cast<int*>(address);

    *test = 1;
    EXPECT_EQ(*static_cast<int*>(address), 1);

    *test = 2;
    EXPECT_EQ(*static_cast<int*>(address), 2);

    EXPECT_TRUE(ShmClose(address, k_shm_size));
    EXPECT_TRUE(ShmDestroy(shm_name.c_str()));
}

TEST(ShmTest, WriteAndReadBetweenTwoProcesses) {
    constexpr std::size_t k_shm_size = 4096;
    constexpr int k_expected_value = 12345;
    constexpr unsigned char k_success = 0;
    constexpr unsigned char k_failure = 1;

    const std::string shm_name = "/shm_test_" + std::to_string(getpid());

    // creator_ready：进程1通知进程2，可以开始 attach。
    int creator_ready[2];
    ASSERT_EQ(pipe(creator_ready), 0);

    // reader_done：进程2通知进程1，读取已经完成。
    int reader_done[2];
    ASSERT_EQ(pipe(reader_done), 0);

    const pid_t creator_pid = fork();
    ASSERT_NE(creator_pid, -1);

    if (creator_pid == 0) {
        close(creator_ready[0]);
        close(reader_done[1]);

        unsigned char creator_status = k_success;

        void* const address = ShmCreate(shm_name.c_str(), k_shm_size);

        if (address == nullptr) {
            creator_status = k_failure;
        } else {
            *static_cast<int*>(address) = k_expected_value;
        }

        // 无论创建成功与否，都要通知进程2，避免其永久等待。
        const bool ready_sent = WriteStatus(creator_ready[1], creator_status);
        close(creator_ready[1]);

        unsigned char reader_status = k_failure;
        const bool reader_reported = ReadStatus(reader_done[0], &reader_status);
        close(reader_done[0]);

        // 必须等进程2完成读取后，进程1才能销毁共享内存。
        bool close_succeeded = true;
        bool destroy_succeeded = true;

        if (address != nullptr) {
            close_succeeded = ShmClose(address, k_shm_size);
            destroy_succeeded = ShmDestroy(shm_name.c_str());
        }

        const bool succeeded = creator_status == k_success && ready_sent && reader_reported
                               && reader_status == k_success && close_succeeded && destroy_succeeded;

        _exit(succeeded ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    const pid_t reader_pid = fork();

    if (reader_pid == -1) {
        // 避免进程1永久等待。
        WriteStatus(reader_done[1], k_failure);

        close(creator_ready[0]);
        close(creator_ready[1]);
        close(reader_done[0]);
        close(reader_done[1]);

        int creator_status = 0;
        waitpid(creator_pid, &creator_status, 0);

        FAIL() << "fork reader process failed";
    }

    if (reader_pid == 0) {
        close(creator_ready[1]);
        close(reader_done[0]);

        unsigned char creator_status = k_failure;
        unsigned char reader_status = k_failure;

        if (ReadStatus(creator_ready[0], &creator_status) && creator_status == k_success) {
            void* const address = ShmAttach(shm_name.c_str(), k_shm_size);

            if (address != nullptr) {
                const int actual_value = *static_cast<const int*>(address);

                const bool close_succeeded = ShmClose(address, k_shm_size);

                if (actual_value == k_expected_value && close_succeeded) {
                    reader_status = k_success;
                }
            }
        }

        close(creator_ready[0]);

        // 即使读取失败，也必须通知进程1执行清理。
        WriteStatus(reader_done[1], reader_status);
        close(reader_done[1]);

        _exit(reader_status == k_success ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    // 父进程不参与共享内存操作。
    close(creator_ready[0]);
    close(creator_ready[1]);
    close(reader_done[0]);
    close(reader_done[1]);

    int creator_status = 0;
    int reader_status = 0;

    ASSERT_EQ(waitpid(creator_pid, &creator_status, 0), creator_pid);
    ASSERT_EQ(waitpid(reader_pid, &reader_status, 0), reader_pid);

    ASSERT_TRUE(WIFEXITED(creator_status));
    ASSERT_TRUE(WIFEXITED(reader_status));

    EXPECT_EQ(WEXITSTATUS(creator_status), EXIT_SUCCESS);
    EXPECT_EQ(WEXITSTATUS(reader_status), EXIT_SUCCESS);
}

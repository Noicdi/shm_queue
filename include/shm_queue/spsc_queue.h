#ifndef SHM_QUEUE_SPSC_QUEUE_H
#define SHM_QUEUE_SPSC_QUEUE_H

#include <stddef.h>

#include "shm_queue/export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct shm_queue_spsc shm_queue_spsc_t;

/* producer */
SHM_QUEUE_LIBRARY_API shm_queue_spsc_t* shm_queue_spsc_create(const char* name, size_t element_size, size_t capacity);
/* copy */
SHM_QUEUE_LIBRARY_API int shm_queue_spsc_push(shm_queue_spsc_t* queue, const void* data);
/* zero-copy */
SHM_QUEUE_LIBRARY_API void* shm_queue_spsc_acquire_write(shm_queue_spsc_t* queue);
SHM_QUEUE_LIBRARY_API int shm_queue_spsc_commit_write(shm_queue_spsc_t* queue);

/* consumer */
SHM_QUEUE_LIBRARY_API shm_queue_spsc_t* shm_queue_spsc_attach(const char* name, size_t element_size);
/* copy */
SHM_QUEUE_LIBRARY_API int shm_queue_spsc_pop(shm_queue_spsc_t* queue, void* data);
/* zero-copy */
SHM_QUEUE_LIBRARY_API const void* shm_queue_spsc_acquire_read(shm_queue_spsc_t* queue);
SHM_QUEUE_LIBRARY_API int shm_queue_spsc_release_read(shm_queue_spsc_t* queue);

/* common */
SHM_QUEUE_LIBRARY_API int shm_queue_spsc_close(shm_queue_spsc_t* queue);
SHM_QUEUE_LIBRARY_API int shm_queue_spsc_destroy(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* SHM_QUEUE_SPSC_QUEUE_H */

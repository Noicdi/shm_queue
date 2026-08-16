#ifndef SHM_QUEUE_EXPORT_H
#define SHM_QUEUE_EXPORT_H

#if defined(__GNUC__) || defined(__clang__)
#define SHM_QUEUE_LIBRARY_API __attribute__((visibility("default")))
#else
#define SHM_QUEUE_LIBRARY_API
#endif

#endif /* SHM_QUEUE_EXPORT_H */

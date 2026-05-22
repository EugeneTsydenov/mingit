#include <stdint.h>
#include <stdio.h>
#include <time.h>

uint64_t hash_commit(const char *parent, time_t timestamp, const char *message,
                     const char *tracked_files) {
    const uint64_t FNV_OFFSET = 14695981039346656037ULL;
    const uint64_t FNV_PRIME = 1099511628211ULL;

    uint64_t hash = FNV_OFFSET;

    char timestamp_str[32];
    snprintf(timestamp_str, sizeof(timestamp_str), "%lld",
             (long long)timestamp);

    const char *parts[] = {parent, timestamp_str, message, tracked_files};

    for (int i = 0; i < 4; i++) {
        const char *s = parts[i];

        while (*s) {
            hash ^= (unsigned char)*s++;
            hash *= FNV_PRIME;
        }
    }

    return hash;
}

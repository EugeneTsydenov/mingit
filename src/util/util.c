#include "util/util.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
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

    hash |= 0x8000000000000000ULL;

    return hash;
}

void hash_to_string(uint64_t hash, char out[17]) {
    snprintf(out, 17, "%llx", (unsigned long long)hash);
}

char *normalize_path(char *path) {
    while (strncmp(path, "./", 2) == 0) {
        path += 2;
    }

    return path;
}

int copy_file(const char *src_path, const char *dest_path) {
    FILE *src = fopen(src_path, "rb");
    if (!src) {
        return 0;
    }

    FILE *dest = fopen(dest_path, "wb");
    if (!dest) {
        fclose(src);
        return 0;
    }

    char buffer[4096];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, bytes_read, dest) != bytes_read) {
            fclose(src);
            fclose(dest);
            return 0;
        }
    }

    fclose(src);
    fclose(dest);

    return 1;
}

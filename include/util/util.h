#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <time.h>

uint64_t hash_commit(const char *parent, time_t timestamp, const char *message,
                     const char *tracked_files);

void hash_to_string(uint64_t hash, char out[17]);

#endif

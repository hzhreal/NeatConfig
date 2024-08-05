#include <stdint.h>
#include <stddef.h>

#include "djb2.h"

size_t djb2_hash(const uint8_t *in, size_t len) {
    size_t hash = 5381;
    while (len--) {
        hash = ((hash << 5) + hash) + *in++;
    }
    return hash;
}
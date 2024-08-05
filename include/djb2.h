#ifndef DJB2_H
#define DJB2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

size_t djb2_hash(const uint8_t *in, size_t len);

#ifdef __cplusplus
}
#endif

#endif // DJB2_H
#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#include <stdint.h>

// Define byte-order macros for STM32 (little-endian system)

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN

// Swap bytes for 16-bit value
static inline uint16_t bswap_16(uint16_t x) {
    return __builtin_bswap16(x);
}

// Swap bytes for 32-bit value
static inline uint32_t bswap_32(uint32_t x) {
    return __builtin_bswap32(x);
}

// Swap bytes for 64-bit value
static inline uint64_t bswap_64(uint64_t x) {
    return __builtin_bswap64(x);
}

// Host to network byte order (big-endian)
#define htobe16(x) (x)
#define htobe32(x) (x)
#define htobe64(x) bswap_64(x)

#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)

#define be16toh(x) (x)
#define be32toh(x) (x)
#define be64toh(x) bswap_64(x)

#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)

#endif // _ENDIAN_H_

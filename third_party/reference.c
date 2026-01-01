#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#if defined(__linux__)
#include <byteswap.h>
#else
#define bswap_16(x) __builtin_bswap16(x)
#define bswap_32(x) __builtin_bswap32(x)
#define bswap_64(x) __builtin_bswap64(x)
#endif

#define parse_uint16(a) bswap_16(*(const uint16_t *)(a))
#define parse_uint32(a) bswap_32(*(const uint32_t *)(a))
#define parse_uint64(a) bswap_64(*(const uint64_t *)(a))

#define parse_ts(a) \
(((uint64_t)bswap_16(*(const uint16_t *)(a)) << 32) | \
 (uint64_t)bswap_32(*(const uint32_t *)((a)+2)))

static inline double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s itch_file\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    const size_t MAX_BYTES = 1ULL << 30;
    unsigned char *buf = aligned_alloc(64, MAX_BYTES);
    if (!buf) {
        perror("alloc");
        return 1;
    }

    size_t bytes_read = fread(buf, 1, MAX_BYTES, f);
    fclose(f);

    const unsigned char *p   = buf;
    const unsigned char *end = buf + bytes_read;

    uint64_t total = 0;
    uint64_t sink  = 0;

    double start = now_sec();

    while (p + 2 <= end) {
        uint16_t len = bswap_16(*(const uint16_t *)p);
        p += 2;

        if (p + len > end)
            break;

        const unsigned char *m = p;
        p += len;

        char t = (char)m[0];
        uint64_t tmp = (unsigned char)t;

        switch (t) {
            case 'A': case 'F': case 'E': case 'C':
            case 'X': case 'D': case 'U': case 'P':
            case 'Q': case 'B': case 'I': case 'N':
                tmp ^= parse_uint64(m + 11);
                break;
            default:
                tmp ^= parse_ts(m + 5);
                break;
        }

        asm volatile("" :: "r,m"(tmp) : "memory");
        sink ^= tmp;
        total++;
    }

    double endt = now_sec();

    double secs   = endt - start;
    double msg_s  = total / secs;
    double ns_msg = (secs * 1e9) / total;

    printf("Parsed bytes:   %.2f MB\n", bytes_read / (1024.0 * 1024.0));
    printf("Messages:       %lu\n", total);
    printf("Time:           %.6f s\n", secs);
    printf("Throughput:     %.3f M msg/s\n", msg_s / 1e6);
    printf("Avg latency:    %.3f ns/msg\n", ns_msg);
    printf("Sink:           %lu\n", sink);

    free(buf);
    return 0;
}


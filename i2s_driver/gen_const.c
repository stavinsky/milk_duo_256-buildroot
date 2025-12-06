#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNK_SAMPLES 4096

static volatile int keep_running = 1;

static void sigint_handler(int sig) {
    (void)sig;
    keep_running = 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <value>\n", argv[0]);
        fprintf(stderr, "  value: constant 32-bit word (e.g. 0x00FF0000 or 123456)\n");
        return 1;
    }

    char* endp = NULL;
    unsigned long v = strtoul(argv[1], &endp, 0);
    if (*endp != '\0') {
        fprintf(stderr, "Invalid value: '%s'\n", argv[1]);
        return 1;
    }

    uint32_t val = (uint32_t)v;

    // 32-bit little-endian representation
    unsigned char sample[4];
    sample[0] = (val >> 0) & 0xFF;
    sample[1] = (val >> 8) & 0xFF;
    sample[2] = (val >> 16) & 0xFF;
    sample[3] = (val >> 24) & 0xFF;

    // Prepare a chunk buffer
    unsigned char buf[CHUNK_SAMPLES * 4];
    for (size_t i = 0; i < CHUNK_SAMPLES; ++i) {
        buf[i * 4 + 0] = sample[0];
        buf[i * 4 + 1] = sample[1];
        buf[i * 4 + 2] = sample[2];
        buf[i * 4 + 3] = sample[3];
    }

    signal(SIGINT, sigint_handler);

    while (keep_running) {
        size_t to_write = sizeof(buf);
        size_t written = 0;

        while (written < to_write && keep_running) {
            ssize_t ret = write(STDOUT_FILENO, buf + written, to_write - written);
            if (ret < 0) {
                if (errno == EINTR)
                    continue;
                fprintf(stderr, "write error: %s\n", strerror(errno));
                return 1;
            }
            if (ret == 0)
                break;
            written += (size_t)ret;
        }
    }

    return 0;
}
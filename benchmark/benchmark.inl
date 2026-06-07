#include <time.h>
#include <stdio.h>

#define BENCHMARK(function, loops) do { \
    struct timespec start, end; \
    int ret; \
    \
    ret = clock_gettime(CLOCK_MONOTONIC, &start); \
    assert(ret == 0 && "clock_gettime failed"); \
    assert(loops > 0); \
    \
    for (int i = 0; i < loops; i++) { \
        function(); \
    } \
    \
    ret = clock_gettime(CLOCK_MONOTONIC, &end); \
    assert(ret == 0 && "clock_gettime failed"); \
    \
    uint64_t start_ns = (uint64_t)start.tv_sec * 1000000000ULL + (uint64_t)start.tv_nsec; \
    uint64_t end_ns = (uint64_t)end.tv_sec * 1000000000ULL + (uint64_t)end.tv_nsec; \
    uint64_t took_ns = end_ns - start_ns; \
    \
    double took_ms = took_ns / 1000000.0; \
    double per_call_ns = took_ns / (double)(loops); \
    \
    printf("Benchmark: %s()\n", #function); \
    printf("  Loops: %d\n", loops); \
    printf("  Total time: %" PRIu64 " ns (%.3f ms)\n", took_ns, took_ms); \
    printf("  Average per call: %.3f ns\n", per_call_ns); \
    printf("---\n"); \
} while (0)
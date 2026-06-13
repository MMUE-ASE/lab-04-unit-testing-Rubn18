/* Linux MAP_32BIT lives behind _GNU_SOURCE; define it before any system header. */
#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include "fake_registers.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <sys/mman.h>
#endif

/* One GPIO register bank spans 0x18..0x1C bytes; pad to a page for safety. */
#define FAKE_BLOCK_SIZE 0x400U

static uint8_t *g_block = NULL;

/*
 * Return a writable block of FAKE_BLOCK_SIZE bytes whose address fits in 32
 * bits, or NULL if the host cannot provide one. The driver receives the base as
 * a uint32_t, so the block MUST live below 4 GB for the address to round-trip.
 */
static uint8_t *allocate_low_block(void)
{
#if UINTPTR_MAX <= 0xFFFFFFFFu
    /* 32-bit host: every address already fits in 32 bits. */
    static uint8_t s_block[FAKE_BLOCK_SIZE];
    return s_block;
#elif defined(_WIN32)
    /* 64-bit Windows: request a base below 4 GB; step up if one is taken. */
    uintptr_t candidate;
    for (candidate = 0x10000000u; candidate < 0x80000000u; candidate += 0x1000000u) {
        void *p = VirtualAlloc((void *)candidate, FAKE_BLOCK_SIZE,
                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (p != NULL) {
            return (uint8_t *)p;
        }
    }
    return NULL;
#elif defined(MAP_32BIT)
    /* 64-bit Linux x86-64: MAP_32BIT guarantees an address in the first 2 GB. */
    void *p = mmap(NULL, FAKE_BLOCK_SIZE, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    return (p == MAP_FAILED) ? NULL : (uint8_t *)p;
#else
    return NULL;
#endif
}

uint32_t fake_gpio_port(void)
{
    if (g_block == NULL) {
        g_block = allocate_low_block();
    }
    /* If this fires, the host placed test memory above 4 GB and a 32-bit port
     * base cannot reach it. The fake_registers strategy needs a sub-4 GB host
     * allocator (see allocate_low_block). */
    assert(g_block != NULL && (uintptr_t)g_block <= 0xFFFFFFFFu);
    return (uint32_t)(uintptr_t)g_block;
}

void fake_registers_reset(void)
{
    uint32_t base = fake_gpio_port();
    memset((void *)(uintptr_t)base, 0, FAKE_BLOCK_SIZE);
}

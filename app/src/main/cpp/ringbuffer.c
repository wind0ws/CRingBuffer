//
// RingBuffer实现
// Created by Threshold on 2018/12/4.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ringbuffer.h"
#include "logger_ringbuf.h"

#ifndef TAKE_MIN
/* take min value of a,b */
#define TAKE_MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif

//config try read/write if no enough data or space
#define RINGBUFFER_CONFIG_TRY_RW_IF_NOT_ENOUGH (0)

/*
** the bitwise version :
** we apply n - 1 mask to n, and then check that is equal to 0
** it will be true for all numbers that are power of 2.
** Lastly we make sure that n is superior to 0.
*/
static inline int is_power_of_2(uint32_t n) {
    return (n > 0 && !(n & (n - 1)));
}

static inline uint32_t roundup_pow_of_two(uint32_t x) {
    if (x == 0 || is_power_of_2(x)) {
        return x;
    }
    // counter divide two times.
    uint32_t counter = 0;
    while (x >>= 1) {
        counter++;
    }
    return (uint32_t) (2 << counter);
}

struct __ring_buffer_t {
    uint32_t in; /*写指针 */
    uint32_t out; /*读指针 */
    uint32_t size; /*环形缓冲区大小 buffer length*/
    char *buf; /*环形缓冲区buffer, 大小一定要为2的n次方才能使用该高级用法 */
};

//buf_size 应是2的n次幂
ring_buf RingBuffer_create(uint32_t buf_size) {
    if (buf_size == 0) {
        return NULL;
    }
    if (!is_power_of_2(buf_size)) {
        RING_LOGW("RingBuffer_create buf_size=%d is not power of 2", buf_size);
        buf_size = roundup_pow_of_two(buf_size);
        RING_LOGW("RingBuffer_create changed buf_size to %d ", buf_size);
    }
    ring_buf ring_buffer_p = calloc(1, sizeof(struct __ring_buffer_t));
    ring_buffer_p->buf = (char *) calloc(buf_size, 1);
    if (!ring_buffer_p->buf) {
        RING_LOGE("can't malloc ring_buffer->buf. size=%d", buf_size);
        free(ring_buffer_p);
        return NULL;
    }
    ring_buffer_p->size = buf_size;
    ring_buffer_p->in = 0;
    ring_buffer_p->out = 0;
    return ring_buffer_p;
}

void RingBuffer_destroy(ring_buf ring_buffer_p) {
    if (!ring_buffer_p) {
        return;
    }
    ring_buffer_p->in = 0;
    ring_buffer_p->out = 0;
    ring_buffer_p->size = 0;

    if (ring_buffer_p->buf) {
        free(ring_buffer_p->buf);
        ring_buffer_p->buf = NULL;
    }
    free(ring_buffer_p);
}

extern inline bool RingBuffer_is_empty(ring_buf ring_buf_p) {
    return RingBuffer_available_data(ring_buf_p) == 0;
}

extern inline bool RingBuffer_is_full(ring_buf ring_buf_p) {
    return RingBuffer_available_space(ring_buf_p) == 0;
}

extern inline void RingBuffer_clear(ring_buf ring_buffer_p) {
    if (ring_buffer_p) {
        ring_buffer_p->in = 0;
        ring_buffer_p->out = 0;
    }
}

extern inline uint32_t RingBuffer_available_data(ring_buf ring_buffer_p) {
    return ring_buffer_p->in - ring_buffer_p->out;
}

extern inline uint32_t RingBuffer_available_space(ring_buf ring_buffer_p) {
    return ring_buffer_p->size - RingBuffer_available_data(ring_buffer_p);
}

uint32_t RingBuffer_write(ring_buf ring_buf_p, const void *source, uint32_t size) {
    if (ring_buf_p == NULL || source == NULL || size == 0) {
        return 0;
    }
    uint32_t start = 0, first_part_len = 0, rest_len = 0;
#if !RINGBUFFER_CONFIG_TRY_RW_IF_NOT_ENOUGH
    uint32_t origin_size = size;
#endif
    uint32_t available_space = RingBuffer_available_space(ring_buf_p);
    size = TAKE_MIN(size, available_space);
#if !RINGBUFFER_CONFIG_TRY_RW_IF_NOT_ENOUGH
    if (origin_size != size) {
        return 0;
    }
#endif
    /* first put the data starting from fifo->in to buffer end */
    start = ring_buf_p->in & (ring_buf_p->size - 1);
    first_part_len = TAKE_MIN(size, ring_buf_p->size - start);
    if (first_part_len) {
        memcpy(ring_buf_p->buf + start, source, first_part_len);
    }
    /* then put the rest_len (if any) at the beginning of the buffer */
    if ((rest_len = size - first_part_len) > 0) {
        memcpy(ring_buf_p->buf, (char *) source + first_part_len, rest_len);
    }
    ring_buf_p->in += size;
    return size;
}

uint32_t RingBuffer_read(ring_buf ring_buf_p, void *target, uint32_t size) {
    if (ring_buf_p == NULL || target == NULL) {
        return 0;
    }
    uint32_t start = 0, first_part_len = 0, rest_len = 0;
#if !RINGBUFFER_CONFIG_TRY_RW_IF_NOT_ENOUGH
    uint32_t origin_size = size;
#endif
    uint32_t available_data = RingBuffer_available_data(ring_buf_p);
    size = TAKE_MIN(size, available_data);
#if !RINGBUFFER_CONFIG_TRY_RW_IF_NOT_ENOUGH
    if (origin_size != size) {
        return 0;
    }
#endif
    /* first get the data from fifo->out until the end of the buffer */
    start = ring_buf_p->out & (ring_buf_p->size - 1);
    first_part_len = TAKE_MIN(size, ring_buf_p->size - start);
    if (first_part_len) {
        memcpy(target, ring_buf_p->buf + start, first_part_len);
    }
    /* then get the rest_len (if any) from the beginning of the buffer */
    if ((rest_len = size - first_part_len) > 0) {
        memcpy((char *) target + first_part_len, ring_buf_p->buf, rest_len);
    }
    ring_buf_p->out += size;
    return size;
}
